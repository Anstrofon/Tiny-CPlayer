#include <iostream>
#define LENGTH_X 100
#define LENGTH_Y 40

#include <string>

#include "ftxui/component/component.hpp"
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include "ftxui/component/screen_interactive.hpp"


#include "taglib/fileref.h"
#include "taglib/tag.h"
#include "FolderViewer.hpp"
#include "SavedMusicList.hpp"


#include <thread>
#include <atomic>
#include <chrono>

using namespace ftxui;

/**
 * The `UserInterface` class is responsible for managing the user interface of an audio player application.
 * It initializes the main components of the UI, including the `FolderViewer`, `AudioPlayer`, and various UI elements.
 * The class also manages a separate thread that regularly updates the UI to reflect the current state of the audio playback.
 */
class UserInterface
{
    cAudio::IAudioSource* mysound{};
    std::shared_ptr<AudioPlayer> _audio_player;
    Component btn_playing;
    Component folder_container;
    Component rendered_ui;
    Component favourites_btn;
    Component favourites_container;
    // initialize the general player's UI
    FolderViewer _folder_viewer;
    SavedMusicList _favourites;
    std::string label = " ▶ ";

    std::string filename;

    std::atomic<bool> running{true}; // Контроль потоку


    void initialize_ui_components(cAudio::IAudioManager* audioMgr)
    {
        File _file(filename);
        _audio_player = std::make_shared<AudioPlayer>(_file, audioMgr);

        btn_playing = _audio_player->get_button();

        _folder_viewer = FolderViewer(_audio_player, _file.get_path());

        _favourites = SavedMusicList(_audio_player);
        favourites_btn = _favourites.get_button();

        // Контейнер для навігації у папках
        folder_container = CatchEvent(_folder_viewer.get_layout(), [&](Event event)
        {
            return _folder_viewer.input_to_filelist_menu(event);
        });
        favourites_container = CatchEvent(_favourites.get_layout(), [&](Event event)
        {
            return _favourites.input_to_favourites_menu(event);
        });
    }
    void create_screen()
    {
        // Об'єднання контейнерів в один
        auto main_container = Container::Vertical(
        {
            btn_playing,
            favourites_btn,
            folder_container,
            favourites_container,
        });
        //final look
        rendered_ui = Renderer(main_container, [&]
        {
            return vbox(
            {hbox(
                {
                btn_playing->Render(),
                gauge(_audio_player->progress_bar())| border | flex,
                text(_audio_player->timer.printElapsedTime(_audio_player->first_run) + "/" + _audio_player->time_stamp_audio())  | border,
                favourites_btn->Render(),
                }),
             text(_audio_player->title) | border,
             folder_container->Render(),
             favourites_container->Render(),
           });
        });
    }

public:

    // initialize the general player's UI
    UserInterface(std::string& filename, cAudio::IAudioManager* audioMgr)
    : filename(filename)
    {

        initialize_ui_components(audioMgr);
        create_screen();

        auto screene = ScreenInteractive::FixedSize(LENGTH_X, LENGTH_Y);

        // Потік для регулярного оновлення
        std::thread([&]()
        {
            while (running.load())
            {
                screene.PostEvent(Event::Custom); // Викликаємо оновлення
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Затримка 50 мс
            }
        }).detach();
        screene.Loop(rendered_ui);  /* yes, it's looping inside the class ui -_- */
    }

    ~UserInterface()
    {
        // Зупиняємо потік після завершення
        running.store(false);
        std::cout << "\033[2J\033[H"; // ANSI-код для очищення екрана
        delete mysound;
    }
};
