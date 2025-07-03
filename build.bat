@echo off
REM Build script for Log2Console Logger library on Windows

setlocal enabledelayedexpansion

REM Default settings
set BUILD_TYPE=Release
set BUILD_EXAMPLES=ON
set GENERATOR="Visual Studio 16 2019"
set SHARED_LIBS=OFF

REM Parse command line arguments
:parse_args
if "%~1"=="" goto end_parse
if /i "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto parse_args
)
if /i "%~1"=="--no-examples" (
    set BUILD_EXAMPLES=OFF
    shift
    goto parse_args
)
if /i "%~1"=="--shared" (
    set SHARED_LIBS=ON
    shift
    goto parse_args
)
if /i "%~1"=="--vs2022" (
    set GENERATOR="Visual Studio 17 2022"
    shift
    goto parse_args
)
if /i "%~1"=="--help" (
    echo Usage: build.bat [options]
    echo Options:
    echo   --debug        Build in debug mode
    echo   --no-examples  Don't build example programs
    echo   --shared       Build shared library instead of static
    echo   --vs2022       Use Visual Studio 2022 generator
    echo   --help         Show this help message
    exit /b 0
)
echo Unknown option: %~1
exit /b 1

:end_parse

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. -G %GENERATOR% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DBUILD_EXAMPLES=%BUILD_EXAMPLES% ^
    -DBUILD_SHARED_LIBS=%SHARED_LIBS%

if errorlevel 1 (
    echo CMake configuration failed!
    exit /b 1
)

REM Build
echo Building...
cmake --build . --config %BUILD_TYPE%

if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build complete!
echo Library: build\%BUILD_TYPE%\log2console.lib (or .dll if shared)
echo Examples: build\%BUILD_TYPE%\example_basic.exe, build\%BUILD_TYPE%\example_wrapper.exe