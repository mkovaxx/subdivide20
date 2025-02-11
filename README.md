# Subdivide 2.0

This is a maintained fork of [Subdivide 2.0](https://cs.nyu.edu/home/people/in_memoriam/biermann/subdivision) from the Media Research Lab at NYU.

It is meant as companion code to the following paper:
```
H. Biermann, A. Levin, D. Zorin.
Piecewise smooth subdivision surfaces with normal control.
SIGGRAPH 2000 Conference Proceedings.
```

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

The build process creates the following outputs:
- `subdivide/examples/ccsub`: Catmull-Clark subdivision tool (CLI).
- `subdivide/examples/loopsub`: Loop subdivision tool (CLI).
- `subdivide/examples/subviewer`: Visual subdivision app (GUI).

## How to Use

All three executables print information about their usage when invoked without arguments.

The `subviewer` GUI app's controls are as follows:
- `<space>`: toggle control mode between `move`/`pick`
  - in `move` mode, dragging with the pointer changes the orientation
  - in `pick` mode: clicking on a control mesh entity toggles its tag
    - `edge` tags: `smooth`/`crease`
    - (boundary) `vertex` tags: `crease`/`corner`
- `S`: increase subdivision depth (number of iterations)
- `W`: write control mesh to file
- `D`: write subdivided mesh to file
