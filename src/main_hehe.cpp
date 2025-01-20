// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <ftxui/component/event.hpp>
#include <string>  // for string, allocator, basic_string
#include <vector>  // for vector

#include "ftxui/component/component.hpp"  // for operator|, Maybe, Checkbox, Radiobox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for Component
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"  // for Element, border, color, operator|, text
#include <filesystem>
#include <iostream>
#include "UserInterface.hpp"

using namespace ftxui;


int main() {

  FolderViewer folder_viewer;

  auto layout = folder_viewer.get_layout();

  auto renderer = layout | CatchEvent([&](Event event) {
                    return folder_viewer.move_in_folders(event);
                  });


  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
