cmake_minimum_required(VERSION 3.11)

project(tiny_cplayer LANGUAGES CXX VERSION 0.2.0)

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
