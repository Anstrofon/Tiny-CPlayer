#include <string>

class File
{
    std::string filename;
    std::string path;
    std::string extension;

public:

    File() = default;

    [[nodiscard]] std::string get_filename() const
    {
        return filename;
    }

    [[nodiscard]] std::string get_path() const
    {
        return path;
    }
    explicit File(const std::string& filename) : filename(filename), path(filename)
    {
        if(filename.find_last_of('/') != std::string::npos)
        {
            this->filename = filename.substr(filename.find_last_of('/') + 1);
            if (filename.find_last_of('.') != std::string::npos)
            {
                extension = filename.substr(filename.find_last_of('.') + 1);
            }
        }
        else
        {
            extension = "";
            this->filename = "";
        }
    }

    bool is_audio()
    {
        return extension == "mp3" || extension == "wav"
                                  || extension == "ogg"
                                  || extension == "raw";
    }
};
