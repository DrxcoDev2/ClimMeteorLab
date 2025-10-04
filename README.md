# ClimMeteorLab

Meteorological software primarily designed to print daily analysis without the need for a large computer.

# Build

## Linux
For installation, you need to have some necessary libraries installed, such as QT and other graphics libraries. You also need OpenGL.
Clone the repository 

- ```shell
  cd ClimMeteorLab
  mkdir -p build
  cd build
  cmake ..
  make
  ./climapp
  ```

## Windows
Windows is unstable

- ```shell
    cd ClimMeteorLab
    mkdir build
    cd build
    cmake .. -G "MinGW Makefiles"
    cmake --build . --config Release
    ./climapp.exe

  ```
