# Array List

This project aims at building custom container types with compiletime allocated memory, that gets along without pointers. That's container types based on arrays.

## Installation
This project is a header only library with only standard dependencies.
Only if you want to run the tests, you need the following deps:
```bash
sudo apt-get install libgtest-dev gcov lcov
```
You can then run a normal cmake build. I use catkin tools for building my project, but you are free to just run
```bash
mkdir build
cd build
cmake ..
make run_tests
```
