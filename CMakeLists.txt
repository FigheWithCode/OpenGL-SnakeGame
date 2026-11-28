cmake_minimum_required(VERSION 3.16)
project(_snake C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glm)

add_executable(GameStart
        src/main.cpp
        src/Shader.cpp
        src/Texture.cpp
        src/Camera.h
        src/Snake.cpp
        src/Game.cpp
        src/Fruit.cpp
        src/stbi_image.cpp
        src/backGround.cpp
        src/GameOver.cpp
        third_party/glad/src/glad.c
)

target_include_directories(GameStart PRIVATE
        third_party/glad/include
        third_party/stb
        src
)

target_link_libraries(GameStart PRIVATE
        glfw
        OpenGL::GL
        glm::glm
)

add_custom_command(TARGET GameStart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Shaders
        $<TARGET_FILE_DIR:GameStart>/Shaders
)
add_custom_command(TARGET GameStart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:GameStart>/Assets
)