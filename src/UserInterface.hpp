#include <cstddef>
#include <iostream>
#define LENGTH_X 100
#define LENGTH_Y 20

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

//#include "AudioPlayer.hpp"

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
    std::string label = " ▶ ";

    std::string filename;
    Component btn_playing;

    cAudio::IAudioSource* mysound{};
    Component rendered_ui;

    // initialize the general player's UI
    FolderViewer _folder_viewer;
    std::shared_ptr<AudioPlayer> _audio_player;


    std::atomic<bool> running{true}; // Контроль потоку

public:

    // initialize the general player's UI
    UserInterface(std::string& filename, cAudio::IAudioManager* audioMgr)
    : filename(filename)
    {
        File _file(filename);
        _audio_player = std::make_shared<AudioPlayer>(_file, audioMgr);

        btn_playing = _audio_player->get_button();

        _folder_viewer = FolderViewer(_audio_player, _file.get_path());

        // Контейнер для навігації у папках
        auto folder_container = CatchEvent(_folder_viewer.get_layout(), [&](Event event)
        {
            return _folder_viewer.input_to_filelist_menu(event);
        });

        // Об'єднання контейнерів в один
        auto main_container = Container::Vertical(
        {
            btn_playing,
            folder_container
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
                }),
             text(_folder_viewer.title) | border,
             folder_container->Render()
           });
        });
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
