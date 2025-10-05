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

## Contribute
Contributions are welcome!
1. Fork the repository.
2. Create a branch: `git checkout -b feature/new-feature`.
3. Submit a pull request.

# Licence
This project is licensed under the GNUv3 license. See [LICENSE](LICENCE)