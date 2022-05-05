Build Platform   | Status
---------------- | ----------------------
MSVC 2019 x64    | [![Build status](https://ci.appveyor.com/api/projects/status/cmh2qiowtm3man71?svg=true)](https://ci.appveyor.com/project/grottansbarn/winmo-bubbles)
Linux x64        | ![Linux x64](https://img.shields.io/bitbucket/pipelines/grottansbarn/winmo_bubbles.svg?style=flat-square)

A copy of one of the best games for the Windows Mobile platform in match3 style - Crazy Bubbles from Pocket Mind company.

![](http://hpc.ru/soft/data/9005/crazybub.gif)

**Dependencies**
cmake 3.16

**Windows:**
WiX Toolset 3.11 

**Build**

- mkdir build

- cd build

- cmake ..

- cmake .. -G "Visual Studio 16 2019" (on Windows)

- cmake --build . --config Release -j6

- cmake --build . --config Release -j6 --target installer-unix-ALL

- cmake --build . --config Release -j6 --target installer-win-ALL (on Windows)
