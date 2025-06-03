cmake_minimum_required(VERSION 3.20)
project(simple_glad_sdl3 C)

# Enable compile_commands.json for LSPs like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use C11
set(CMAKE_C_STANDARD 11)

# Automatically collect all .c files in src/
file(GLOB SRC_FILES src/*.c)

# Create the executable
add_executable(program.exe ${SRC_FILES})

# Add include directory for GLAD
target_include_directories(program.exe PRIVATE include)

# Add SDL3 include & lib paths manually
target_include_directories(program.exe PRIVATE /usr/local/include)
target_link_directories(program.exe PRIVATE /usr/local/lib)

# Link against SDL3, OpenGL, and dl (for glad on Linux)
target_link_libraries(program.exe
    SDL3
    GL
    dl
)


