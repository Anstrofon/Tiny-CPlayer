#include <algorithm>
#include <cstdlib>
#include <ftxui/component/event.hpp>
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

    std::size_t number_of_files_in_directory()
    {
        using std::filesystem::directory_iterator;
        return std::distance(directory_iterator(directorypath), directory_iterator{});
    }

    void set_title(const std::string& tit)
    {
        TagLib::FileRef file(directorypath.c_str());

        directorypath.remove_filename();
        if(file.tag()->title().isEmpty())
        {
            title = tit;
        }
        else 
        {
            title = file.tag()->title().toCString(true);
        }
    }


    void set_file_list()
    {   
        if (!filelist.empty()) 
        { 
            filelist.clear();
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
public:

    FolderViewer()
    {
    }

    std::string title = " ";

    Component get_layout()
    {
        return Container::Vertical(
 {
            Checkbox("󰷏 Show folders", &menu_2_show),
            Menu(&filelist, &selected_file) | border | Maybe(&menu_2_show),
        });
    }

    bool move_in_filelist(Event& event)
    {   
        if(selected_file == 0 && !hidden.empty() && (event == Event::k || event == Event::ArrowUp))
        {
            selected_file = 0;
            
            filelist.insert(filelist.begin(), hidden.back());
            hidden.pop_back(); 
            return true;
        }
        if(selected_file > 10 && (event == Event::j || event == Event::ArrowDown))
        {   
            //selected_file = 10;
            
            hidden.emplace_back(filelist[0]);
            filelist.erase(filelist.begin());

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
                File file(directorypath.string() + "/" + filelist[selected_file]);
                
                if(file.is_audio())
                {   
                    if(observer.lock())
                    {
                        observer.lock()->change_file(file);
                    }

                    set_title(filelist[selected_file]);
                }
                else 
                {
                    directorypath.append(filelist[selected_file]);
                }
                return true;
            }
            hidden.clear();
            set_file_list();
            return true;
        }

        return false;
    }

    FolderViewer(const std::shared_ptr<AudioPlayer>& observer, const std::string& input_path) : observer(observer),
    directorypath(input_path)
    {
        if(this->directorypath.is_relative())
        {
            this->directorypath = std::filesystem::absolute(this->directorypath);
            if (input_path == ".")
            {
                move_backward();
            }
        }
        set_file_list();
    }
};
