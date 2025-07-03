cmake_minimum_required(VERSION 3.10)
project(Log2ConsoleLogger VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option to build shared library
option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(BUILD_EXAMPLES "Build example programs" ON)

# Platform detection for compiler flags
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(UNIX AND NOT APPLE)
    # Linux specific flags
    find_package(Threads REQUIRED)
endif()

# Source files for the library
set(LIBRARY_SOURCES
    Log2ConsoleCommon.cpp
    Log2ConsoleUdpClient.cpp
    Logger.cpp
    PlatformUtils.cpp
    SocketPlatform.cpp
)

# Header files for the library
set(LIBRARY_HEADERS
    Log2ConsoleCommon.h
    Log2ConsoleUdpClient.h
    Logger.h
    LoggerWrapper.h
    PlatformUtils.h
    SocketPlatform.h
)

# Create the library
add_library(log2console ${LIBRARY_SOURCES})

# Set include directories
target_include_directories(log2console
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Link platform-specific libraries
if(WIN32)
    target_link_libraries(log2console PUBLIC ws2_32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(log2console PUBLIC Threads::Threads)
endif()

# Set properties for the library
set_target_properties(log2console PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${LIBRARY_HEADERS}"
)

# Build examples if requested
if(BUILD_EXAMPLES)
    # Example 1: Basic usage
    add_executable(example_basic example.cpp)
    target_link_libraries(example_basic PRIVATE log2console)
    
    # Example 2: Wrapper usage
    add_executable(example_wrapper example_wrapper.cpp)
    target_link_libraries(example_wrapper PRIVATE log2console)
endif()

# Installation rules
include(GNUInstallDirs)

install(TARGETS log2console
    EXPORT log2console-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/log2console
)

# Export targets
install(EXPORT log2console-targets
    FILE log2console-targets.cmake
    NAMESPACE log2console::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/log2console
)

# Create and install package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/log2console-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/log2console-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/log2console
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/log2console-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/log2console-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/log2console-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/log2console
)

# Print configuration summary
message(STATUS "")
message(STATUS "Log2Console Logger Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build shared libs: ${BUILD_SHARED_LIBS}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")