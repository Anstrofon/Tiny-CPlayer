cmake_minimum_required(VERSION 3.11)

project(tiny_cplayer LANGUAGES CXX VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

find_package(Taglib QUIET)
find_package(TagLib QUIET)

if(NOT TagLib_FOUND AND NOT TAGLIB_FOUND)
    message(STATUS "TagLib not found on system. Fetching from GitHub...")
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        taglib
        GIT_REPOSITORY https://github.com/taglib/taglib.git
        GIT_TAG        v2.0.1
    )
    FetchContent_MakeAvailable(taglib)

    # TagLib includes headers flatly when installed, but in source tree they reside in subfolders.
    # Export all source subdirectories to target 'tag' interface for FetchContent builds:
    target_include_directories(tag INTERFACE
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/toolkit>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/asf>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/mpeg>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/ogg>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/flac>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/mpc>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/mp4>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/ape>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/wavpack>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/trueaudio>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/riff>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/mod>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/s3m>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/it>
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib/xm>
        $<BUILD_INTERFACE:${taglib_BINARY_DIR}>
        $<BUILD_INTERFACE:${taglib_BINARY_DIR}/taglib>
    )
endif()

add_executable(main_enter src/main.cpp)
target_include_directories(main_enter PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/libs/miniaudio)

target_link_libraries(main_enter
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE tag
)

# Installation settings
# install(TARGETS main_enter RUNTIME DESTINATION bin)

# --- Testing ---
set(BUILD_TESTING ON CACHE BOOL "" FORCE)
include(CTest)
option(TINY_CPLAYER_TESTS "Build tests for tiny_cplayer" ON)
if(TINY_CPLAYER_TESTS)

    # Fetch GoogleTest
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # File Unit Test
    add_executable(test_file tests/test_file.cpp)
    target_include_directories(test_file PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(test_file gtest_main)
    add_test(NAME test_file COMMAND test_file)

    # Timer Unit Test
    add_executable(test_timer tests/test_timer.cpp)
    target_include_directories(test_timer PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(test_timer gtest_main)
    add_test(NAME test_timer COMMAND test_timer)

    # Audio Metadata Integration Test
    add_executable(test_audio_metadata tests/test_audio_metadata.cpp)
    target_include_directories(test_audio_metadata PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(test_audio_metadata gtest_main tag)
    add_test(NAME test_audio_metadata COMMAND test_audio_metadata)
endif()
