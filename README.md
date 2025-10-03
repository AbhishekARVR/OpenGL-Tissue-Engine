# OpenGL Tissue Engine

A real-time soft-body physics simulation engine built with OpenGL, designed to simulate deformable tissue-like materials with realistic mesh deformation and cloth-like behavior.

## Overview

This project implements a physics-based simulation system that models soft tissue behavior using spring-mass dynamics. The engine features an interactive 3D mesh that responds to forces and movement, creating realistic deformation effects suitable for medical simulation, game development, or physics research.

## Features

### Core Simulation
- **Soft-Body Physics**: Spring-mass system for realistic tissue deformation
- **Real-Time Rendering**: Hardware-accelerated OpenGL rendering pipeline
- **Mesh Deformation**: Dynamic vertex manipulation with proper deformation on movement
- **Cloth-Like Behavior**: Flexible material simulation with natural movement

### Technical Implementation
- **Separated Buffer Architecture**: Independent position and color buffers for efficient GPU operations
- **Custom Mesh Abstraction**: Modular mesh creation and management system
- **Shader-Based Rendering**: GLSL shaders for vertex and fragment processing
- **CMake Build System**: Cross-platform build configuration

### Graphics Pipeline
- Modern OpenGL rendering
- Vertex and fragment shader support
- Efficient buffer management (VBO/VAO)
- Real-time mesh updates

## Project Structure

```
OpenGL-Tissue-Engine/
├── main.cpp              # Application entry point and main loop
├── mesh.h                # Mesh abstraction and geometry management
├── shader.h              # Shader loading and compilation utilities
├── CMakeLists.txt        # Build configuration
├── shaders/              # GLSL shader files
│   ├── vertex.glsl       # Vertex shader
│   └── fragment.glsl     # Fragment shader
├── external/             # Third-party libraries (GLFW, GLAD, GLM)
├── build/                # Build output directory
├── Images/               # Screenshots and documentation images
└── .vscode/              # IDE configuration
```

## Development Setup

### Prerequisites

- **C++ Compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **CMake**: Version 3.10 or higher
- **OpenGL**: Version 3.3 or higher
- **Graphics Driver**: Updated GPU drivers with OpenGL support

### Dependencies

The project uses the following libraries (included in `external/`):
- **GLFW**: Window creation and input handling
- **GLAD**: OpenGL extension loading
- **GLM**: Mathematics library for graphics

### Building the Project

#### macOS/Linux

```bash
# Clone the repository
git clone https://github.com/AbhishekARVR/OpenGL-Tissue-Engine.git
cd OpenGL-Tissue-Engine

# Create build directory
mkdir -p build
cd build

# Generate build files and compile
cmake ..
make

# Run the application
./main
```

#### Windows

```bash
# Clone the repository
git clone https://github.com/AbhishekARVR/OpenGL-Tissue-Engine.git
cd OpenGL-Tissue-Engine

# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake ..

# Open the solution in Visual Studio and build
# Or build from command line:
cmake --build . --config Release
```

## Usage

### Running the Simulation

1. **Launch the application**: Execute the compiled binary from the build directory
2. **Interact with the mesh**: Use mouse/keyboard controls to manipulate the tissue
3. **Observe deformation**: Watch real-time physics simulation and mesh response

### Controls

- **Mouse**: Rotate camera/view
- **Keyboard**: Movement and interaction (specific bindings in source)
- **ESC**: Exit application

### Configuration

Simulation parameters can be adjusted in `main.cpp`:
- Mesh resolution and density
- Spring constants and damping factors
- Rendering settings
- Time step and physics iterations

## Technical Details

### Physics Model

The engine uses a spring-mass system where:
- Vertices act as point masses
- Springs connect adjacent vertices
- Forces include spring forces, damping, and gravity
- Numerical integration updates positions each frame

### Rendering Pipeline

1. **Mesh Generation**: Create vertex positions and topology
2. **Buffer Setup**: Upload geometry to GPU
3. **Shader Processing**: Transform vertices and compute colors
4. **Rasterization**: Draw triangles to screen
5. **Dynamic Updates**: Modify buffers for animation

### Mesh Abstraction

The `mesh.h` header provides:
- Flexible mesh creation functions
- Vertex attribute management
- Buffer binding utilities
- Deformation and update methods

## Recent Improvements

- ✅ Fixed mesh deformation on movement (Oct 3, 2025)
- ✅ Cloth-like behavior implementation (Sep 27, 2025)
- ✅ Separated position and color buffers (Sep 27, 2025)
- ✅ Extracted mesh abstraction layer (Sep 26, 2025)
- ✅ Implemented mesh creation function (Sep 26, 2025)
- ✅ CMake build system integration (Sep 25, 2025)

## Future Enhancements

### Physics
- [ ] Collision detection and response
- [ ] Self-collision handling
- [ ] Variable stiffness materials
- [ ] Tearing and cutting mechanics
- [ ] External force application (wind, user interaction)

### Rendering
- [ ] Normal mapping for surface detail
- [ ] Physically-based rendering (PBR)
- [ ] Shadow casting
- [ ] Ambient occlusion
- [ ] Post-processing effects

### Features
- [ ] Multiple object simulation
- [ ] Save/load simulation states
- [ ] Parameter tuning UI
- [ ] Performance profiling tools
- [ ] Benchmark mode

### Optimization
- [ ] Spatial hashing for broad-phase collision
- [ ] GPU-accelerated physics computation
- [ ] Multi-threading support
- [ ] Level-of-detail (LOD) system
- [ ] Compute shader integration

## Performance Considerations

- Mesh complexity directly affects frame rate
- Physics iterations trade accuracy for speed
- Buffer updates should minimize CPU-GPU transfers
- Consider using instancing for multiple objects

## Troubleshooting

### Build Issues
- Ensure CMake can find OpenGL libraries
- Check that external dependencies are properly included
- Verify compiler supports C++11 or later

### Runtime Issues
- Update graphics drivers for OpenGL support
- Check console output for shader compilation errors
- Ensure working directory contains shader files

## Contributing

Contributions are welcome! Areas of interest:
- Physics algorithm improvements
- Rendering optimizations
- Cross-platform compatibility
- Documentation and examples
- Bug fixes and testing

## License

This project is private and not currently licensed for public use.

## Acknowledgments

- OpenGL community for extensive documentation
- GLFW and GLAD for windowing and extension loading
- Graphics programming resources and tutorials

## Contact

Developed by [AbhishekARVR](https://github.com/AbhishekARVR)

---

**Note**: This is an ongoing research and development project. Features and implementation details are subject to change.
