# Casteljau's Algorithm Demo
A very simple application that highlights how De-Casteljau's algorithm works by showing the intermediate lines used by the algorithm to calculate the resulting bezier curve.

# Requirements
In order to compile this demo, you need to have:
* CMake 3.2.2 or a newer version (Note: actually even much older versions like CMake-2.8 also work and if you really need to use an older version simply update the ```cmake_minimum_required``` property in your CMake file accordingly).
* Qt-5.7 or a newer version.
* A C++ Compiler with C++-14 support, such as GCC-4.9 or newer.

# How to build it
1. Make sure the above mentioned requirements are met.
2. Clone the repository or download an archive and unpack it, then change into the directory.
3. ```$ mkdir build```
4. ```$ cd build```
5. ```$ cmake ../```
6. ```$ make```

The resulting executable can then be launched by issuing ```./src/de\_casteljau\_demo``` inside the build directory. If CMake fails to find Qt, you may use the ```CMAKE_PREFIX_PATH``` environment variable to help cmake locate the correct version of Qt. Running CMake would then look like this: ```$ CMAKE_PREFIX_PATH="<path-to-the-Qt5.7-cmake-directory>" cmake ..```.

# Screenshot
![de-casteljaus-screenshot](https://cloud.githubusercontent.com/assets/12494/19405574/52dd2d04-9279-11e6-8729-fd90d52e7835.png)

# License
The application is released under the terms of the GPLv3 or any later edition.
