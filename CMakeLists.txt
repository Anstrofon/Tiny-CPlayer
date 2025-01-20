cmake_minimum_required(VERSION 3.11)

project(tiny_cplayer LANGUAGES CXX VERSION 1.0.0)

include(FetchContent)

# Fetch and make available FTXUI
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)



# Include cAudio custom CMakeLists.txt
add_subdirectory(${CMAKE_SOURCE_DIR}/cAudio )

#add_subdirectory(${CMAKE_SOURCE_DIR}/taglib)

# Find and include OpenAL library
find_package(OpenAL REQUIRED)
if(NOT OpenAL_FOUND)
    set(OPENAL_INCLUDE_DIR /usr/include/AL)
    set(OPENAL_LIBRARY /usr/lib/libopenal.so)
endif()



find_package(Taglib REQUIRED)
# Add the main executable
add_executable(main_enter src/main.cpp)


# Link libraries: cAudio, FTXUI, OpenAL
target_link_libraries(main_enter
  PRIVATE cAudio
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  ${OPENAL_LIBRARY}
  PRIVATE cAp_mp3Decoder
  PRIVATE tag
)
# Installation settings
install(TARGETS main_enter RUNTIME DESTINATION bin)
