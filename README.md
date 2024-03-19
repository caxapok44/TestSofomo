# TestAssigmentSofomo

## Introduction

TestAssigmentSofomo is a Qt-based desktop application developed as part of a coding assignment. This application demonstrates the use of Qt for building GUI applications and interacting with databases and external APIs.

## Requirements

- Linux OS
- Qt 5.x (5.15 or later recommended)
- GCC or another compatible C++ compiler
- SQLite3 (for database functionality)

## Pre-Installation

Before compiling and running the project, you need to set up the necessary environment and dependencies. Run the provided pre-installation script located in the `script` directory:

```bash
chmod +x script/pre-install.sh
./script/pre-install.sh
```
This script will install the required packages and set up the environment for the application.

## Compiling and Building
To compile and build the project, follow these steps:

1.Open a terminal and navigate to the project directory.

2.Run qmake to generate the Makefile:

```bash
qmake
```
3.Build the project using make:

```bash
make
```

After a successful build, the executable TestAssigmentSofomo will be created in the build directory.

## Running the Application
Run the application by executing the binary from the terminal:

```bash
./TestAssigmentSofomo
```
##Usage
[Provide instructions on how to use the application, any command-line arguments it accepts, what it does, etc.]

Troubleshooting
If you encounter issues with missing dependencies or libraries, ensure that the pre-install.sh script has been executed correctly.
For problems related to Qt not being found, verify that Qt is correctly installed and that your PATH environment variable includes the Qt bin directory.