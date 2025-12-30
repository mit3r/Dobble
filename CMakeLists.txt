cmake_minimum_required(VERSION 3.12)

# Global settings
project(Dobble)


# Compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -Wpedantic)

# Define macros based on build type
add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

# Include directories
include_directories(include)
include_directories(src)
include_directories(src/client)

# Dependencies
find_package(Threads REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebEngineWidgets)

# Qt MOC configuration (only MOC needed, no QML/UI files)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

# Sources

file(GLOB SERVER_COMMON_SOURCES "src/server/common/*.cpp")

# Game Server
file(GLOB_RECURSE GAMESERVER_SPECIFIC "src/server/gameserver/*.cpp")
add_executable(dobble_gameserver
    ${GAMESERVER_SPECIFIC}
    ${SERVER_COMMON_SOURCES}
)
target_link_libraries(dobble_gameserver Threads::Threads)

# Lobby Server
file(GLOB_RECURSE LOBBYSERVER_SPECIFIC "src/server/lobbyserver/*.cpp")
add_executable(dobble_lobbyserver
    ${LOBBYSERVER_SPECIFIC}
    ${SERVER_COMMON_SOURCES}
)
target_link_libraries(dobble_lobbyserver Threads::Threads)

# Web Client
add_custom_target(build_web_client
    COMMAND npm run build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ui
    COMMENT "Building Web Client"
)

# Tools - Game Test Client
add_executable(game_test_client
    src/tools/game_test_client.cpp
    ${SERVER_COMMON_SOURCES}
)
target_link_libraries(game_test_client Threads::Threads)

# Client
file(GLOB_RECURSE CLIENT_SOURCES "src/client/*.cpp" "src/client/*.hpp")
qt_standard_project_setup()
qt_add_executable(
    dobble_client
    ${CLIENT_SOURCES}
)

# Better include paths for Qt autogen files
target_include_directories(dobble_client PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/client
    ${CMAKE_SOURCE_DIR}/include
)
add_dependencies(dobble_client build_web_client)
target_link_libraries(dobble_client PRIVATE Qt6::Core Qt6::Widgets Qt6::WebEngineWidgets)