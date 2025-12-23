cmake_minimum_required(VERSION 3.12)

# Global settings
project(Dobble)


# Compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -Wpedantic)

# Include directories
include_directories(include)
include_directories(src)
include_directories(src/client)

# Dependencies
find_package(Threads REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebEngineWidgets)


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
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/web
    COMMENT "Building Web Client"
)

# Client
file(GLOB_RECURSE CLIENT_SOURCES "src/client/*.cpp" "src/client/*.hpp")
qt_standard_project_setup()
qt_add_executable(
    dobble_client
    ${CLIENT_SOURCES}
)
add_dependencies(dobble_client build_web_client)
target_link_libraries(dobble_client PRIVATE Qt6::Core Qt6::Widgets Qt6::WebEngineWidgets)