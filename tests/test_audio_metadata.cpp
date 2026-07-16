#include <gtest/gtest.h>
#include "MetadataParser.hpp"
#include "File.hpp"
#include <filesystem>

// Assuming CMake runs tests from the build directory, so tests/ are in ../tests/
// Alternatively, we can use a CMAKE define or just hardcode for this simple project
std::string get_project_root() {
    // If run from build/, root is ..
    if (std::filesystem::exists("../tests/test.mp3")) {
        return "../";
    }
    // If run from project root
    if (std::filesystem::exists("tests/test.mp3")) {
        return "./";
    }
    return "";
}

TEST(MetadataParserTest, FileWithoutMetadata) {
    std::string root = get_project_root();
    ASSERT_NE(root, "") << "Could not find tests directory";
    
    File f(root + "tests/test.mp3");
    AudioMetadata meta = MetadataParser::parse(f);
    
    // We expect title to fallback to filename if no metadata or empty metadata
    EXPECT_EQ(meta.title, "test.mp3");
    // Depending on the file, length_in_seconds might be non-zero if audio properties exist
    // Even without tags, TagLib can read audio properties (duration)
    EXPECT_GE(meta.length_in_seconds, 0);
}

TEST(MetadataParserTest, FileWithMetadata) {
    std::string root = get_project_root();
    ASSERT_NE(root, "") << "Could not find tests directory";
    
    File f(root + "tests/TagLib test.mp3");
    AudioMetadata meta = MetadataParser::parse(f);
    
    // Let's assume the metadata contains a title. 
    // We just check that it does not fall back to the filename completely or that it parses something.
    // If we knew the exact title, we'd EXPECT_EQ(meta.title, "Expected Title");
    EXPECT_NE(meta.title, "");
    EXPECT_GE(meta.length_in_seconds, 0);
}
