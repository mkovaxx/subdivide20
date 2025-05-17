# Subdivide 2.0

This is a **maintained** fork of [Subdivide 2.0](https://cs.nyu.edu/home/people/in_memoriam/biermann/subdivision) from the Media Research Lab at NYU.

It is meant as companion code to the following [paper](doc/piecewise-smooth.pdf):
```
H. Biermann, A. Levin, D. Zorin.
Piecewise smooth subdivision surfaces with normal control.
SIGGRAPH 2000 Conference Proceedings.
```

## Features

- **Modern Build System**: Uses CMake for cross-platform building
- **Cross-Platform**: Supports Linux and macOS (Windows support coming soon)
- **Modern C++**: Updated to C++11 standard
- **CI/CD**: Automated testing with GitHub Actions
- **Improved Documentation**: Comprehensive build and usage instructions

## Features

### Subdivision Algorithms
- **Catmull-Clark Subdivision**: For quadrilateral meshes
- **Loop Subdivision**: For triangular meshes
- **Piecewise Smooth Subdivision**: With normal control

### Interactive Viewer
- Real-time visualization of subdivision surfaces
- Interactive mesh manipulation
- Support for creases and corners
- Export capabilities for both control and subdivided meshes

### Visual Examples

Complex geometrical features:

![examples of complex features](doc/images/5.png)

Normal interpolation:

![normal interpolation](doc/images/6.png)

Precise surface manipulation at corners:

![surface manipulation with corners](doc/images/8.png)

## Building from Source

### Prerequisites
- CMake 3.15 or later
- C++17 compatible compiler (GCC 8+, Clang 10+, Apple Clang 12+)
- OpenGL and GLUT (for the viewer application)

### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y cmake g++ freeglut3-dev

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -- -j$(nproc)
```

### macOS
```bash
# Install dependencies using Homebrew
brew install cmake freeglut

# Build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -- -j$(nproc)
```

### Build Options
- `-DCMAKE_BUILD_TYPE=Release` (default): Optimized release build
- `-DCMAKE_BUILD_TYPE=Debug`: Build with debug symbols
- `-DBUILD_TESTS=ON`: Build test suite (coming soon)

## Usage

### Command Line Tools

#### Catmull-Clark Subdivision (`ccsub`)
```bash
# Show help
./build/bin/ccsub --help

# Subdivide a mesh
./build/bin/ccsub input.obj -o output.obj -s 3
```

#### Loop Subdivision (`loopsub`)
```bash
# Show help
./build/bin/loopsub --help

# Subdivide a mesh
./build/bin/loopsub input.obj -o output.obj -s 2
```

### Interactive Viewer (`subviewer`)

```bash
# Launch the viewer
./build/bin/subviewer [input.obj]
```

#### Viewer Controls
- **Left Mouse Button**: Rotate view
- **Right Mouse Button**: Pan view
- **Mouse Wheel**: Zoom in/out
- **Space**: Toggle control mode between `move`/`pick`
  - `move`: Drag to change view orientation
  - `pick`: Click to toggle mesh entity tags
    - Edge tags: `smooth`/`crease`
    - Boundary vertex tags: `crease`/`corner`
- **S**: Increase subdivision depth
- **W**: Write control mesh to file
- **D**: Write subdivided mesh to file

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- Original work by H. Biermann, A. Levin, and D. Zorin
- Maintained by the open source community
