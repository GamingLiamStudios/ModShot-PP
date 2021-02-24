# ModShot-PP
OneShot remade in C++. To be used for making OneShot mods.

This project started as a way for me to create OneShot mods without learning Ruby or RPG Maker XP. 
This project is not affiliated with [ModShot-Core](https://github.com/Astrabit-ST/ModShot-Core) or any similar projects.

## Features
* Speed
* Efficent use of memory
* Map Editing*
* Full C++ scripting*
* Easy porting of Original OneShot data

\*To be implemented

## Building
Prerequisites for Building: [CMake](https://cmake.org/download/) & [SFML](https://github.com/SFML/SFML/tree/2.5.1)

First, Clone the Repository
```bash
git clone https://github.com/GamingLiamStudios/ModShot-PP.git
cd ModShot-PP
mkdir build
cd build
```
Then generate a project file or makefile for your platform. If you want to use a particular IDE, make sure it is installed; don't forget to set the Start-Up Project in Visual Studio or the Target in Xcode.
```bash
# UNIX Makefile
cmake ..

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 16" ..
cmake -G "Visual Studio 16 Win64" ..
...
```
If you compile and run, you should now have a (mostly)functioning copy of ModShot-PP, ready for creating your own OneShot Mods.
