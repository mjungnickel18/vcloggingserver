#!/bin/bash
# Build script for Log2Console Logger library

# Default build type
BUILD_TYPE="Release"
BUILD_EXAMPLES="ON"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --no-examples)
            BUILD_EXAMPLES="OFF"
            shift
            ;;
        --shared)
            BUILD_SHARED="-DBUILD_SHARED_LIBS=ON"
            shift
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --debug        Build in debug mode"
            echo "  --no-examples  Don't build example programs"
            echo "  --shared       Build shared library instead of static"
            echo "  --help         Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DBUILD_EXAMPLES=${BUILD_EXAMPLES} \
    ${BUILD_SHARED}

# Build
echo "Building..."
cmake --build . --config ${BUILD_TYPE} -j$(nproc)

echo "Build complete!"
echo "Library: build/liblog2console.a (or .so if shared)"
echo "Examples: build/example_basic, build/example_wrapper"