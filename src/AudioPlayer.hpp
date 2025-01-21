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

#include "cAudio/cAudio.h" /* usr/local/include/cAudio */

#include "taglib/fileref.h"
#include "taglib/tag.h"

#include <chrono>
#include "Timer.hpp"
#include "File.hpp"

class AudioPlayer
{
    std::string label = " ▶ ";

    File filename;
    int final_seconds = 0;
    cAudio::IAudioSource* mysound;
    
    ftxui::Component btn_playing;
    cAudio::IAudioManager* audioMgr;


public:
    bool first_run;
    Timer timer;
    TagLib::AudioProperties *properties;

    void rewind(const float&& seconds)
    {   
        if (mysound == nullptr) return;
        
        float current_seconds = timer.elapsedSeconds();

        if (current_seconds + seconds > final_seconds || current_seconds + seconds < 0)  return;

        mysound->seek(seconds, true);
        timer.add_seconds(seconds);
    }
    float progress_bar(bool set_progress = false)
    {   
        if (mysound == nullptr) return 0;

        TagLib::FileRef file(filename.get_path().c_str());
        TagLib::AudioProperties *properties = file.audioProperties();

        final_seconds = properties->lengthInSeconds();

        int current_seconds = static_cast<int>(timer.elapsedSeconds());

        float progress = static_cast<float>(current_seconds) / final_seconds;

        if (progress == 1 || first_run) /* stop player, when sound ends */
        {
            label = " ▶ ";
            progress = 0;
            timer.pause();
            first_run = true;
        }
        return progress;
    }

    std::string time_stamp_audio()
    {   
        if(mysound == nullptr) return "00:00";

        TagLib::FileRef file(filename.get_path().c_str());

        properties = file.audioProperties();

        int seconds = properties->lengthInSeconds() % 60;
        int minutes = (properties->lengthInSeconds() - seconds) / 60;

        std::string result;
        if(minutes < 10)
        {
            result = "0" + std::to_string(minutes) + ":";
        }
        else 
        {
            result = std::to_string(minutes) + ":";
        }

        if(seconds < 10)
        {
            result += "0" + std::to_string(seconds);
        }
        else 
        {
            result += std::to_string(seconds);
        }

        return result; 
    }

    void play_button()
    {   
        if (mysound == nullptr) return;

        if (label == " ▶ ")
        {
            mysound->play2d(false);
            if (first_run)
            {
                timer.reset();
                first_run = false;
            }
            else
            {
                timer.resume();
            }

            label = " ⏸ ";
        }
        else
        {
            mysound->pause();
            timer.pause();
            label = " ▶ ";
        }
    }
    
    AudioPlayer(const File& filename, cAudio::IAudioManager* audioMgr) : 
        filename(filename), audioMgr(audioMgr), first_run(true)
    {   
        if(this->filename.is_audio())
        {
            mysound = audioMgr->create("song", filename.get_path().c_str(),true);
        }
        else
        {
            mysound = nullptr;
        }
        
        //set button interactions
        btn_playing = ftxui::Container::Horizontal({
        Button(" << ", [&] {rewind(-5); }, ftxui::ButtonOption::Border()),
        Button(&label, [this] { play_button(); }, ftxui::ButtonOption::Border()),
        Button(" >> ", [&] { rewind(5); }, ftxui::ButtonOption::Border() )
        });
    }
    
    void change_file(const File& filename)
    {
        this->filename = filename;
        
        if (mysound != nullptr)
        {
            if(!mysound->isStopped()) mysound->stop();
        }
        
        mysound = audioMgr->create("song", filename.get_path().c_str(),true);
        timer.reset();
        first_run = true;
    }
    
    ftxui::Component get_button()
    {
        return btn_playing;
    }
    
};