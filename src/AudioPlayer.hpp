#pragma once
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

#include "miniaudio.h"

#include "taglib/fileref.h"
#include "taglib/tag.h"

#include "Timer.hpp"
#include "File.hpp"


struct AudioEngine
{
    bool available = false;
    bool is_stopped = true;
    bool sound_initialized = false;

    AudioEngine()
    {
        available = ma_engine_init(NULL, &engine) == MA_SUCCESS;
    }

    ~AudioEngine()
    {
        if (sound_initialized)
        {
            ma_sound_uninit(&sound); // Спочатку видаляємо звук
        }
        if(available)
        {
            ma_engine_uninit(&engine);
        }
    }

    void set_file(const File& file)
    {
        if (sound_initialized)
        {
            ma_sound_uninit(&sound);
        }

        ma_result result = ma_sound_init_from_file(&engine, file.get_path().c_str(), 0, NULL, NULL, &sound);
        sound_initialized = (result == MA_SUCCESS);
    }

    void stop()
    {
        is_stopped = true;
        ma_sound_reset_start_time(&sound);
    }

    void play()
    {
        ma_sound_start(&sound);
        is_stopped = false;
    }

    void seek(const float& seconds)
    {
        ma_uint64 current_frame;
        ma_sound_get_cursor_in_pcm_frames(&sound, &current_frame);
        ma_sound_seek_to_pcm_frame(&sound, current_frame + (44100 * seconds));
    }

    void pause()
    {
        ma_sound_stop(&sound);
        is_stopped = true;
    }
private:
    File file;
    ma_engine engine;
    ma_sound sound;
};

class AudioPlayer
{
    ftxui::Component btn_playing;
    AudioEngine mysound;
    File filename;
    std::string label = " ▶ ";
    int final_seconds = 0;

public:

    [[nodiscard]] File get_file() const
    {
        return filename;
    }
    bool first_run;
    Timer timer;
    TagLib::AudioProperties *properties;

    std::string title = " ";

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

    void rewind(const float&& seconds)
    {
        if (!mysound.available || !mysound.sound_initialized) return;

        float current_seconds = timer.elapsedSeconds();

        if (current_seconds + seconds > final_seconds || current_seconds + seconds < 0)  return;

        mysound.seek( /*current_seconds +*/ seconds);
        timer.add_seconds(seconds);
    }


    float progress_bar(bool set_progress = false)
    {
        if (!mysound.available || !mysound.sound_initialized) return 0;

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
        if(!mysound.available || !mysound.sound_initialized)
        {
            return "00:00";
        }

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
        if (!mysound.available || !mysound.sound_initialized) return;

        if (label == " ▶ ")
        {
            mysound.play();
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
            mysound.pause();
            timer.pause();
            label = " ▶ ";
        }
    }

    AudioPlayer(const File& filename) :
        filename(filename), first_run(true)
    {
        if(this->filename.is_audio())
        {
            mysound.set_file(filename);
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

        if (mysound.available)
        {
            if(!mysound.is_stopped)
            {
                mysound.stop();
            }
        }

        mysound.set_file(filename);
        timer.reset();
        first_run = true;
    }

    ftxui::Component get_button()
    {
        return btn_playing;
    }

    ~AudioPlayer()
    {
        if (mysound.available)
        {
            mysound.stop();
        }
    }
};
