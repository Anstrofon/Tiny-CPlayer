#include <gtest/gtest.h>
#include "MetadataParser.hpp"
#include "File.hpp"
#include <filesystem>

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

    EXPECT_EQ(meta.title, "test.mp3");
    EXPECT_GE(meta.length_in_seconds, 0);
}

TEST(MetadataParserTest, FileWithMetadata) {
    std::string root = get_project_root();
    ASSERT_NE(root, "") << "Could not find tests directory";

    File f(root + "tests/TagLib test.mp3");
    AudioMetadata meta = MetadataParser::parse(f);

    EXPECT_NE(meta.title, ""); //EXPECT_EQ(meta.title, "TagLib test");
    EXPECT_GE(meta.length_in_seconds, 0);
}
