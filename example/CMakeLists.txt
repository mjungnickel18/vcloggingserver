cmake_minimum_required(VERSION 3.10)
project(Log2ConsoleExample)

# Find the log2console library
find_package(log2console REQUIRED)

# Create an example application
add_executable(myapp main.cpp)

# Link against the log2console library
target_link_libraries(myapp PRIVATE log2console::log2console)

# Enable logging (optional - can be controlled per build)
target_compile_definitions(myapp PRIVATE ENABLE_LTC_LOGGING)