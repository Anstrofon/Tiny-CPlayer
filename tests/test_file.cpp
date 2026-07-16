#include <gtest/gtest.h>
#include "File.hpp"

TEST(FileTest, ParsePathAndExtension) {
    File f("/home/user/music/song.mp3");
    EXPECT_EQ(f.get_filename(), "song.mp3");
    EXPECT_EQ(f.get_path(), "/home/user/music/song.mp3");
}

TEST(FileTest, ParseWithoutExtension) {
    File f("/home/user/music/song");
    EXPECT_EQ(f.get_filename(), "song");
    EXPECT_EQ(f.get_path(), "/home/user/music/song");
    EXPECT_FALSE(f.is_audio());
}

TEST(FileTest, ParseWithoutDirectory) {
    File f("just_audio.wav");
    // According to current File.hpp implementation, if there is no '/', 
    // extension="" and filename=""
    EXPECT_EQ(f.get_filename(), "");
    EXPECT_EQ(f.get_path(), "just_audio.wav");
}

TEST(FileTest, IsAudioLogic) {
    EXPECT_TRUE(File("/music/song.mp3").is_audio());
    EXPECT_TRUE(File("/music/song.wav").is_audio());
    EXPECT_TRUE(File("/music/song.ogg").is_audio());
    EXPECT_TRUE(File("/music/song.raw").is_audio());

    EXPECT_FALSE(File("/music/doc.pdf").is_audio());
    EXPECT_FALSE(File("/music/doc.txt").is_audio());
    EXPECT_FALSE(File("/music/video.mp4").is_audio());
}
