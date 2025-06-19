
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include "AudioPlayer.hpp"
#include "ftxui/component/component.hpp"  // for operator|, Maybe, Checkbox, Radiobox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"

using namespace ftxui;

class SavedMusicList
{
    std::vector<std::string> favourites;
    std::vector<std::string> hidden;
    int selected_file = -1;
    std::weak_ptr<AudioPlayer> observer;
    bool menu_2_show = false;
    std::string label_heart = "";

    void reload_txt()
    {
        const char* home = getenv("HOME");
        std::filesystem::path path = std::filesystem::path(home) / ".local/share/terminal-audioplayer";
        std::filesystem::create_directories(path); // якщо ще нема, створюємо
        std::string list = (path / "favorites.txt").string();
        std::ofstream text(list);
        if(text.is_open())
        {
            for(const auto& file : favourites)
            {
                text << file << '\n';
            }
            text.close();
        }
    }
    void button_favourites()
    {
        File file = observer.lock()->get_file();
        if(file.is_audio())
        {
            if (label_heart == "󰋑")
            {
                remove_favourite(file);
                label_heart = "";

            }
            else
            {
                add_favourite(file);
                label_heart = "󰋑";
            }
        }
    }

    void remove_favourite(const File& file)
    {
        std::string path = file.get_path();
        auto it = std::find(favourites.begin(), favourites.end(), path);
        if (it != favourites.end())
        {
            favourites.erase(it);
            reload_txt();
            favourites.clear();
            load_favourites();
        }
    }

    bool scroll_down()
    {
        hidden.emplace_back(favourites[0]);
        favourites.erase(favourites.begin());
        return true;
    }

    bool scroll_up()
    {
        selected_file = 0;
        favourites.insert(favourites.begin(), hidden.back());
        if (!hidden.empty()) hidden.pop_back();
        return true;
    }


// TODO: розробити функцію для читання-збереження в файл
    void save_favourites(const File& file)
    {
        // TODO: зберегти в файл
        const char* home = getenv("HOME");
        std::filesystem::path path = std::filesystem::path(home) / ".local/share/terminal-audioplayer";
        std::filesystem::create_directories(path); // якщо ще нема, створюємо
        std::string list = (path / "favorites.txt").string();
        std::ofstream text(list);
        if(text.is_open())
        {
            text << file.get_path() << '\n';
            text.close();
        }

    }

    void load_favourites()
    {
        // TODO: завантажити з файлу
        const char* home = getenv("HOME");

        std::filesystem::path path = std::filesystem::path(home) / ".local/share/terminal-audioplayer";
        std::filesystem::create_directories(path); // якщо ще нема, створюємо

        std::string list = (path / "favorites.txt").string();
        std::ifstream text(list);
        if(text.is_open())
        {
            std::string line;
            while(std::getline(text, line))
            {
                //File file(line);
                if (!line.empty())
                {
                    favourites.emplace_back(line);
                }
            }
            text.close();
        }


    }
    void add_favourite(const File& file)
    {
        favourites.emplace_back(file.get_path());
        save_favourites(file);
    }
public:
    SavedMusicList() = default;

    SavedMusicList(const std::shared_ptr<AudioPlayer>& observer) : observer(observer)
    {
        load_favourites();
    }

    bool input_to_favourites_menu(Event& event)
    {
        if(!menu_2_show)
        {
            return false;
        }
        if (event == Event::l  || event == Event::ArrowRight)
        {
            interact_with_selected_file();
            return true;
        }
        if (selected_file > 10 && !favourites.empty() && (event == Event::j  || event == Event::ArrowDown))
        {
            return scroll_down();
        }
        if (selected_file == 0 && !hidden.empty()  && (event == Event::k  || event == Event::ArrowUp))
        {
            return scroll_up();
        }
        return false;
    }

    Component get_button()
    {
        return Container::Vertical(
        {
            Button(&label_heart, [this] { button_favourites(); }, ButtonOption::Border()),
        });
    }

    Component get_layout()
    {
        return Container::Vertical(
        {
            Checkbox("󰷏 Favourites", &menu_2_show),
            Menu(&favourites, &selected_file) | border | Maybe(&menu_2_show),
        });
    }

    void interact_with_selected_file()
    {
        if(favourites.empty())
        {
            return;
        }
        File file_(favourites.at(selected_file));
        if(observer.lock())
        {
            observer.lock()->change_file(file_);
            observer.lock()->set_title(file_);
        }
        label_heart = "󰋑";
    }
};
