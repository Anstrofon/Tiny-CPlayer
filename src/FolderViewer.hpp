#include <algorithm>
#include <cstdlib>
#include <ftxui/component/event.hpp>
#include <iostream>
#include <memory>
#include "AudioPlayer.hpp"
#include <string>
#include <utility>

#include <vector>
#include <filesystem>
#include "ftxui/component/component.hpp"  // for operator|, Maybe, Checkbox, Radiobox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"

using namespace std::filesystem;
using namespace ftxui;


class FolderViewer
{
    std::string folder_icon = " ";

    std::vector<std::string> filelist;
    std::vector<std::string> hidden;
    path directorypath =  ".";

    int steps_below = 0;

    int selected_file = -1;
    bool menu_2_show = false;
    std::weak_ptr<AudioPlayer> observer;

    void set_title(const File& file)
    {
        TagLib::FileRef fileref(file.get_path().c_str());
        if(fileref.tag()->title().isEmpty())
        {
            title = file.get_filename();
        }

        else
        {
            title = fileref.tag()->title().toCString(true);
        }
    }


    void set_file_list()
    {
        if (!filelist.empty())
        {
            filelist.clear();
        }
        if(!hidden.empty())
        {
            hidden.clear();
        }

        std::vector<std::string> folders;
        for (const auto& entry : directory_iterator(directorypath))
        {
            File file(entry.path().string());

            if (!is_directory(entry) && file.is_audio())
            {
                filelist.emplace_back(entry.path().filename().string());
            }
            else if(is_directory(entry))
            {
                folders.emplace_back( entry.path().filename());
            }
        }
        for(const std::string& folder : folders)
        {
            filelist.emplace_back(folder_icon + folder);
        }
        std::reverse(filelist.begin(), filelist.end());
        filelist.emplace_back("..");
    }

    void move_backward()
    {
        directorypath.remove_filename();
        std::string temp = directorypath.string(); //  костиль
        temp.pop_back();
        directorypath = temp;
    }

    void scroll_up()
    {
        selected_file = 0;
        filelist.insert(filelist.begin(), hidden.back());
        hidden.pop_back();
    }

    void scroll_down()
    {
        hidden.emplace_back(filelist[0]);
        filelist.erase(filelist.begin());
    }

    void interact_with_selected_file()
    {
        File file(directorypath.string() + "/" + filelist[selected_file]);

        if(file.is_audio() && observer.lock())
        {
            observer.lock()->change_file(file);
            set_title(file);
        }
        else
        {
            std::string folder =  filelist[selected_file].substr(folder_icon.length());
            directorypath.append(folder);
        }
    }

    void set_path(std::string path)
    {
        if(!std::filesystem::exists(directorypath))
        {
            std::cerr << "Directory does not exist: " << directorypath << '\n';
            path = ".";
        }

        if(this->directorypath.is_relative())
        {
            this->directorypath = std::filesystem::absolute(this->directorypath);
            if (path == "." || path == "..")
            {
                move_backward();
            }
        }
    }
public:

    FolderViewer()
    = default;

    std::string title = " ";

    Component get_layout()
    {
        return Container::Vertical(
 {
            Checkbox("󰷏 Show folders", &menu_2_show),
            Menu(&filelist, &selected_file) | border | Maybe(&menu_2_show),
        });
    }

    bool input_to_filelist_menu(Event& event)
    {
        if(selected_file == 0 && !hidden.empty() && (event == Event::k || event == Event::ArrowUp))
        {
            scroll_up();
            return true;
        }
        if(selected_file > 10 && (event == Event::j || event == Event::ArrowDown))
        {
            scroll_down();
            return true;
        }

        if (event == Event::l  || event == Event::ArrowRight)
        {
            if (filelist[selected_file] == "..")
            {
                if(directorypath.string() == "/")
                {
                    return false;
                }
                move_backward();
            }
            else
            {
                interact_with_selected_file();
            }
            set_file_list();
            return true;
        }
        return false;
    }

    FolderViewer(const std::shared_ptr<AudioPlayer>& observer, const std::string& input_path) : observer(observer),
    directorypath(input_path)
    {
        set_path(input_path);
        set_file_list();
    }
};
