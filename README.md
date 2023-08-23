# Casteljau's Algorithm Demo
A very simple application that highlights how De-Casteljau's algorithm works by showing the intermediate lines used by the algorithm to calculate the resulting bezier curve.

# Requirements
In order to compile this demo, you need to have:
* CMake 3.17.0 or a later version (Note: actually even much older versions like CMake-2.8 may also work and if you really need to use an older version simply update the ```cmake_minimum_required``` property in your CMake file accordingly).
* Qt-6.5 or later, or Qt-5.7 or later if you enable BUILD_WITH_QT5 for example by running cmake with -DBUILD_WITH_QT5=ON.
* A C++ Compiler with C++-17 support, such as GCC-9 or later.

# How to build it
1. Make sure the above mentioned requirements are met.
2. Clone the repository or download an archive and unpack it, then change into the directory.
3. ```$ mkdir build```
4. ```$ cd build```
5. ```$ cmake ../``` (you may also have to add -DCMAKE_PREFIX_PATH or -DBUILD_WITH_QT5 with appropriate values depending on your environment
6. ```$ make```

The resulting executable can then be launched by issuing ```./src/de\_casteljau\_demo``` inside the build directory. If CMake fails to find Qt, you may use the ```CMAKE_PREFIX_PATH``` environment variable to help cmake locate the correct version of Qt. Running CMake would then may look like this for Qt5 ```$ cmake -DCMAKE_PREFIX_PATH=~/Qt/5.15.2/gcc_64 -DBUILD_WITH_QT5=ON ..``` or the following for Qt6: ```$ cmake -DCMAKE_PREFIX_PATH=~/Qt/6.5.2 ..``` (assuming your Qt versions are installed under `~/Qt`).

# Screenshot
![de-casteljaus-screenshot](https://cloud.githubusercontent.com/assets/12494/19405574/52dd2d04-9279-11e6-8729-fd90d52e7835.png)

# License
The application is released under the terms of the GPLv3 or any later edition.
