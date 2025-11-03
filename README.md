# Planar Geometry

* This library and example programs are built using [Bazel](https://bazel.build/).
* Written in C++20 with only using standard library.
    - No external dependency.
* For unit test, the library requires Google Test.

## List of Example Programs

* case_studies/random_polygon: creates a random polygon without self intersection.
* case_studies/triangulate: triangulates polygons and calculates area.

## Build and Run

To build:
```shell
bazel build //case_studies:random_polygon
bazel build //case_studies:triangulate
```

To run:
```shell
bazel-bin/case_studies/random_polygon
bazel-bin/case_studies/triangulate
```

In Windows, use `\` instead of `/`, obviously.

**Note**: This library was tested under the following environments

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

The program read the csv files in `polygons` directory and generates corresponding output files in
plain TeX (NOT a LaTeX) source with brief print out in the terminal.

**Note**: if `__DEBUG_TIKZ__` is defined (currently it is defined in `core/polygon.cc`) the program
also generates another TeX file named as `debug.tex` which shows the progress of triangulation
at every step.

The output files contain primitive plain TeX commands with
[TikZ](https://github.com/pgf-tikz/pgf) macros.
(TikZ is a set of commands to draw graphics with PGF as the backend of the drawing system.)

The program draws the perimeter polygon and triangles in thick (0.8pt) and very thin (0.1pt)
lines, respectively.

The figure below show the progress of triangulation of the concave polygon defined in `random_polygon_0.csv`:
![Progress](progress.gif)
