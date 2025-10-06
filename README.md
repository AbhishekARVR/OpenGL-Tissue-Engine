# OpenGL Tissue Engine

A real-time soft-body physics simulation engine built with OpenGL, designed to simulate deformable tissue-like materials with realistic mesh deformation and cloth-like behavior.

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

## Technical Details

## Recent Improvements

- ✅ Fixed mesh deformation on movement (Oct 3, 2025)
- ✅ Cloth-like behavior implementation (Sep 27, 2025)
- ✅ Separated position and color buffers (Sep 27, 2025)
- ✅ Extracted mesh abstraction layer (Sep 26, 2025)
- ✅ Implemented mesh creation function (Sep 26, 2025)
- ✅ CMake build system integration (Sep 25, 2025)


## Contact

Developed by [AbhishekARVR](https://github.com/AbhishekARVR)

---

**Note**: This is an ongoing research and development project. Features and implementation details are subject to change.
