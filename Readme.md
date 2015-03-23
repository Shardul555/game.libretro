# game.libretro

Libretro compatibility layer for the Kodi Game API

# Building out-of-tree (recommended)

## Linux

Clone the repo and create a build directory

```shell
git clone https://github.com/kodi-game/game.libretro.git
cd game.libretro
mkdir build
cd build
```

Generate a build environment with config for debugging

```shell
cmake -DADDONS_TO_BUILD=game.libretro \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=$HOME/workspace/xbmc/addons \
      $HOME/workspace/xbmc/project/cmake/addons
```

If you are developing in Eclipse, you can create a "makefile project with existing code" using `game.libretro/` as the existing code location. To build, enter Properties -> "C/C++ Build" and change the build command to `make -C build`.

It is also possible to generate Eclipse project files with cmake

```shell
cmake -G"Eclipse CDT4 - Unix Makefiles" \
      -D_ECLIPSE_VERSION=4.4 \
      -DADDONS_TO_BUILD=game.libretro \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=$HOME/workspace/xbmc/addons \
      $HOME/workspace/xbmc/project/cmake/addons
```

# Building in-tree (cross-compiling)

Kodi's build system will fetch the add-on from the GitHub URL and git hash specified in [game.libretro.txt](https://github.com/garbear/xbmc/blob/retroplayer-15alpha2/project/cmake/addons/addons/game.libretro/game.libretro.txt).

## Linux

Ensure that kodi has been built successfully. Then, from the root of the source tree, run

```shell
make install DESTDIR=$HOME/kodi
```

Build the add-on

```shell
make -C tools/depends/target/binary-addons PREFIX=$HOME/kodi ADDONS="game.libretro"
```

The compiled .so can be found at

```
$HOME/kodi/lib/kodi/addons/game.libretro/game.libretro.so
```

To rebuild the add-on or compile a different one, clean the build directory

```shell
make -C tools/depends/target/binary-addons clean
```

## Windows

We will use CMake to generate a `kodi-addons.sln` Visual Studio solution and project files. Add-ons can be built individually through their specific project, or all at once by building the solution.

First, download and install [CMake](http://www.cmake.org/download/).

Run the script from [PR 6658](https://github.com/xbmc/xbmc/pull/6658) to create Visual Studio project files

```
tools\windows\prepare-binary-addons-dev.bat
```

The generated solution can be found at

```
project\cmake\addons\build\kodi-addons.sln
```

No source code is downloaded at the CMake stage; when the project is built, the add-on's source will be downloaded and compiled.

## OSX

Per [README.osx](https://github.com/garbear/xbmc/blob/retroplayer-15alpha2/docs/README.osx), enter the `tools/depends` directory and make the add-on:

```shell
cd tools/depends
make -C target/binary-addons ADDONS="game.libretro"
```

To rebuild the add-on or compile a different one, clean the build directory

```shell
make -C target/binary-addons clean
```
