spheres Game Engine Framework
=====
[![Build Status](https://travis-ci.org/ThomasHauth/spheres.svg?branch=master)](https://travis-ci.org/ThomasHauth/spheres)

spheres is a lightweight C++11 framework that provides functionality for render, entity management, sound, physics and animation purposes. I supports OpenGL on Linux and the GoogleVR android platform.


Linux Build
----

To build the Spheres engine, unit test and benchmark suite on Linux, run the following commands:

````
./distributeAssets.sh
cd build/
cmake ../src
make -j2
SpheresTest/SpheresTest
cd SpheresTestBench/
./SpheresTestBench
````

To generate the documentation with doxygen, run
doxygen spheresDoxygenDocs



Android VR Build
-----
To checkout the Android VR Build and example Activity, either open the folder build_androidvr in Android Studio or execute:

````
cd build_androidvr/
./get_externals.sh
./gradlew

````

