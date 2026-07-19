# Tiny-CPlayer

A simple C++ TUI player

## Getting Started
To get started, clone the repository and build the project using CMake.
Before building, make sure you have the required system libraries installed.
#### Ubuntu / Debian / Linux Mint:
```
sudo apt update
sudo apt install -y build-essential cmake git libasound2-dev
```
#### Arch Linux / Manjaro:
```
sudo pacman -S --needed base-devel cmake git alsa-lib
```
#### Fedora:
```
sudo dnf install -y gcc-c++ cmake git alsa-lib-devel
```
#### macOS:
```
# Якщо ще не встановлено Xcode Command Line Tools та CMake:
xcode-select --install
brew install cmake git
```
Clone the repository:

```
git clone https://github.com/anstrofon/tiny-cplayer
```
Next, navigate to the project directory and build it using CMake.
```
cd tiny-cplayer
cmake -B build -DTINY_CPLAYER_TESTS=OFF # Disable tests
make -C build main_enter
```
Now you can run the player:
```
./build/main_enter
```

## Libraries
- FTXUI (UI) https://github.com/ArthurSonzogni/FTXUI
- TagLib (metadata parsing) https://github.com/taglib/taglib
- Miniaudio (audio-engine) https://github.com/ryanisaacg/miniaudio
