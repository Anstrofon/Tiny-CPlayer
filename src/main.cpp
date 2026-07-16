#define MINIAUDIO_IMPLEMENTATION
#include "libs/miniaudio/miniaudio.h"
#include "UserInterface.hpp"
#include <iostream>
#include <cstdlib>


int main(int argc, char* argv[])
{
  std::string filename = ".";
  if (argc > 1)
  {
    filename = argv[1];
  }
  UserInterface user_interface(filename);

  return EXIT_SUCCESS;
}
