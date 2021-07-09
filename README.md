# Cartera
Cartera is a cross-platform portfolio management software

## Build
To build `cartera`, a C++ compiler that supports C++17 is required.

### Dependencies
 - CMake >= 3.10
 - Boost >= 1.75 (json, system, filesystem)
 - OpenSSL
 - Qt >= 5.12
 - KDE Frameworks 5 >= 5.76 (kirigami2, ki18n)

### Instructions
Like any CMake-based projects,

```sh
mkdir build && cd build
cmake ..
make -j
```

Note: On Windows, you can either open the project in Visual Studio, or use NMake.

To run all the unit tests, run `ctest` in the building directory.

## Development
`cartera` is layered to avoid being tightly coupled with Qt framework:
 - `core` library contains the low-level functionalities and should not depend on Qt for any reasons.
 - `coreqt` library contains the necessary code to integrate `core` into Qt.
 - `contents` directory includes the QML UI code.
 - `main.cpp` is for the main application that instantiates required resources and launches the UI.

## License
LGPLv3
