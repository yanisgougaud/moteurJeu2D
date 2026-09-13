cmake_minimum_required(VERSION 4.0)

project(moteurJeu2D
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
)

FetchContent_MakeAvailable(SFML)

add_executable(moteurJeu2D
    src/main.cpp
    engine2D/src/components/collider/collider.cpp
    engine2D/src/components/sprite/sprite.cpp
)

target_include_directories(moteurJeu2D PRIVATE
    engine2D/include
)

target_link_libraries(moteurJeu2D PRIVATE
    SFML::Graphics
)

add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
)