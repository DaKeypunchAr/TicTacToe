cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(TicTacToe VERSION 0.0.0 LANGUAGES CXX)

add_executable(TicTacToe "src/Main.cpp" "src/TicTacToe/TicTacToe.cpp" "src/ShaderProgram/Program.cpp" "src/Texture/Texture.cpp" "src/TicTacToe/Players/Human.cpp" "src/TicTacToe/Board/Board.cpp" "src/UI/Button/Button.cpp" "src/TextRenderer/TextRenderer.cpp")

set(GLFW_LIBRARY_TYPE STATIC)
set(GLFW_BUILD_SHARED_LIBS OFF)

add_subdirectory("glew-2.1.0/build/cmake")
add_subdirectory("glfw-3.4")
add_subdirectory("glm")

target_link_libraries(TicTacToe PUBLIC glew_s glfw glm "${CMAKE_SOURCE_DIR}/FT/freetype.lib")

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
configure_file("src/root.hpp.in" root.hpp @ONLY)

target_include_directories(TicTacToe PUBLIC 
    "include"
    "glew-2.1.0/include"
    "glfw-3.4/include"
    "FT/include"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(TicTacToe PUBLIC _DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(TicTacToe PUBLIC _RELEASE)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_compile_definitions(TicTacToe PUBLIC _32BIT)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(TicTacToe PUBLIC _64BIT)
endif()