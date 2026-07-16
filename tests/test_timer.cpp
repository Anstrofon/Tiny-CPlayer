#include <gtest/gtest.h>
#include "Timer.hpp"
#include <thread>
#include <chrono>

TEST(TimerTest, InitialState) {
    Timer t;
    bool first_run = true;
    EXPECT_EQ(t.printElapsedTime(first_run), "00:00");
    first_run = false;
    EXPECT_EQ(t.printElapsedTime(first_run), "00:00");
}

TEST(TimerTest, PauseLogic) {
    Timer t;
    t.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    t.pause();
    float elapsed1 = t.elapsedSeconds();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    float elapsed2 = t.elapsedSeconds();
    
    // When paused, time should not accumulate
    EXPECT_FLOAT_EQ(elapsed1, elapsed2);
    
    t.resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    float elapsed3 = t.elapsedSeconds();
    EXPECT_GT(elapsed3, elapsed2);
}

TEST(TimerTest, AddSetSeconds) {
    Timer t;
    t.reset();
    t.set_seconds(10.0f);
    EXPECT_GE(t.elapsedSeconds(), 10.0f);
    
    t.add_seconds(5.0f);
    EXPECT_GE(t.elapsedSeconds(), 15.0f);
}

TEST(TimerTest, TimeFormatting) {
    Timer t;
    t.reset();
    t.pause();
    t.set_seconds(65.0f); // 1 minute 5 seconds
    bool first_run = false;
    EXPECT_EQ(t.printElapsedTime(first_run), "01:05");
    
    t.set_seconds(3600.0f); // 60 minutes
    EXPECT_EQ(t.printElapsedTime(first_run), "60:00");
}
