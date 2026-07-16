#pragma once

#include <string>
#include "File.hpp"
#include "taglib/fileref.h"
#include "taglib/tag.h"
#include "taglib/audioproperties.h"

struct AudioMetadata {
    std::string title;
    int length_in_seconds;
};

class MetadataParser {
public:
    static AudioMetadata parse(const File& file) {
        AudioMetadata metadata;
        metadata.title = file.get_filename();
        metadata.length_in_seconds = 0;

        TagLib::FileRef fileref(file.get_path().c_str());
        if (fileref.isNull() || fileref.tag() == nullptr) {
            return metadata;
        }

        if (!fileref.tag()->title().isEmpty()) {
            metadata.title = fileref.tag()->title().toCString(true);
        }

        TagLib::AudioProperties *properties = fileref.audioProperties();
        if (properties) {
            metadata.length_in_seconds = properties->lengthInSeconds();
        }

        return metadata;
    }
};
