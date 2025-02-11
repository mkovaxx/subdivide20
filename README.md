# Subdivide 2.0

This is a **maintained** fork of [Subdivide 2.0](https://cs.nyu.edu/home/people/in_memoriam/biermann/subdivision) from the Media Research Lab at NYU.

The improvements contributed here include:
- build support for Linux and macOS
- compatibility with modern C++ compilers: Clang and GCC
- conformance with C++11
- reformatted with a modern C++ style and readability in mind
- Markdown-ified README files

It is meant as companion code to the following [paper](doc/piecewise-smooth.pdf):
```
H. Biermann, A. Levin, D. Zorin.
Piecewise smooth subdivision surfaces with normal control.
SIGGRAPH 2000 Conference Proceedings.
```

## Features

Complex geometrical features:

![examples of complex features](doc/images/5.png)

Normal interpolation:

![normal interpolation](doc/images/6.png)

Precise surface manipulation at corners:

![surface manipulation with corners](doc/images/8.png)

## How to Build

The following platforms are currently supported:
- Linux (Ubuntu)
- macOS

Support for Windows is coming soon!

Each platform has a corresponding `Makefile.$PLATFORM` inside the `subdivide` directory.

For example, to build for macOS, execute the following command:
```sh
make -C subdivide -f Makefile.macos
```

## How to Use

The build process creates the following outputs:
- `subdivide/examples/ccsub`: Catmull-Clark subdivision tool (CLI).
- `subdivide/examples/loopsub`: Loop subdivision tool (CLI).
- `subdivide/examples/subviewer`: Visual subdivision app (GUI).

All three executables print information about their usage when invoked without arguments.

The `subviewer` GUI app's controls are as follows:
- `<space>`: toggle control mode between `move`/`pick`
  - in `move` mode, dragging with the pointer changes the orientation
  - in `pick` mode: clicking on a control mesh entity toggles its tag
    - edge tags: `smooth`/`crease`
    - (boundary) vertex tags: `crease`/`corner`
- `S`: increase subdivision depth (number of iterations)
- `W`: write control mesh to file
- `D`: write subdivided mesh to file
