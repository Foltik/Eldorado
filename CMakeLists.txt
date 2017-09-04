cmake_minimum_required(VERSION 3.7)
project(eldorado)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

include_directories(/usr/include/freetype2/)

set(SOURCE_FILES
        src/body.cpp
        src/body.h
        src/earthframe.cpp
        src/earthframe.h
        src/main.cpp
        src/menuframe.cpp
        src/menuframe.h)

add_executable(eldorado ${SOURCE_FILES})

target_link_libraries(eldorado GL GLEW glfw assimp SOIL freetype k5)

add_custom_command(TARGET eldorado POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        $<TARGET_FILE_DIR:eldorado>/models)

add_custom_command(TARGET eldorado POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:eldorado>/shaders)

add_custom_command(TARGET eldorado POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:eldorado>/textures)

add_custom_command(TARGET eldorado POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts
        $<TARGET_FILE_DIR:eldorado>/fonts)
