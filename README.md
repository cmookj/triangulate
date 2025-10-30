# Triangulation: C++ Geometry

* This project is built using [Bazel](https://bazel.build/) but can be easily built using other
terminal based tools, e.g., CMake or IDE.
* This program is written in C++20 with only using standard library.
    - No external dependency is required for the program itself.
* For unit test, this project requires Google Test.

## Build and Run

To build:
```shell
bazel build //main:triangulate
```

To run:
```shell
bazel-bin/main/triangulate
```

In Windows, use `\` instead of `/`.

Result:
```shell
Area = 0.65
Area = 1269.19
```

**Note**: This code was tested under the following environments

* macOS 15.7.1 with Apple clang compiler 17.0.0
* Ubuntu 22.04 (running as a container image) with gcc 11.4.0
* Windows 11 with Visual Studio 2022

## Unit Tests

This program includes basic unit tests using google test.

```shell
bazel test //test:basic_test
```

In Windows, use the following command:

```shell
bazel test --cxxopt=/std:c++20 --host_cxxopt=/std:c++20 //test:basic_test
```

## Output Files

The program read the csv files in `examples` directory.
It generates two output files in plain TeX (NOT a LaTeX) source with brief print out in the terminal.

* `example_1.tex`
* `example_2.tex`

**Note**: if `__DEBUG_TIKZ__` is defined (currently it is defined in `core/polygon.cc`) the program
also generates another TeX file named as `debug.tex` which shows the progress of triangulation
at every step.  The file `progress.gif` shows the triangulation progress for
the input file `example_2.csv`.

The two files contains primitive plain TeX commands with [TikZ](https://github.com/pgf-tikz/pgf) macros.
(The TikZ is a set of commands to draw graphics with PGF as the backend of the drawing system.)

The program draws the perimeter polygon and triangles in thick (0.8pt) and very thin (0.1pt)
lines, respectively.

The figure below show the progress of triangulation of the concave polygon defined in `example_2.csv`:
![Progress](progress.gif)
