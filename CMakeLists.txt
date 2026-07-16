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
endif()

add_executable(main_enter src/main.cpp)
target_include_directories(main_enter PRIVATE ${CMAKE_SOURCE_DIR}/libs/miniaudio)

target_link_libraries(main_enter
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE tag
)

# Installation settings
# install(TARGETS main_enter RUNTIME DESTINATION bin)
