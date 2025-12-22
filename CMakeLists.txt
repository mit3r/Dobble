cmake_minimum_required(VERSION 3.10)
project(Dobble)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
add_compile_options(-Wall -Wextra -Wpedantic )

# Include directories
include_directories(include)

# --- Server ---
file(GLOB_RECURSE SERVER_SOURCES "src/server/*.cpp")
add_executable(dobble_server ${SERVER_SOURCES})
target_include_directories(dobble_server PRIVATE src/server)

# --- Client ---
# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB_RECURSE CLIENT_SOURCES "src/client/*.cpp")
add_executable(dobble_client ${CLIENT_SOURCES})
target_include_directories(dobble_client PRIVATE src/client)
target_link_libraries(dobble_client sfml-graphics sfml-window sfml-system)
