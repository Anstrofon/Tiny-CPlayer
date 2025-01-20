#include "cAudio/cAudio.h" /* usr/local/include/cAudio */
#include <cAudio/IPluginManager.h>
#include "UserInterface.hpp"
#include <cstdlib>


int main(int argc, char* argv[])
{
  //Create an uninitialized Audio Manager
  cAudio::IAudioManager* audioMgr = cAudio::createAudioManager(false);

  //Get the Plugin Manager for mp3 decoding
  cAudio::IPluginManager* pPluginManager = cAudio::getPluginManager();
  pPluginManager->installPlugin("libcAp_mp3Decoder.so");
  audioMgr->initialize();


  std::string filename = ".";
  if (argc > 1)
  {
    filename = "/";
    filename =+ argv[1];
  }
  UserInterface user_interface(filename, audioMgr);

  cAudio::destroyAudioManager(audioMgr);
  return EXIT_SUCCESS;
}
