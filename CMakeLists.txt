cmake_minimum_required(VERSION 3.10)
project(opengl_learn_project VERSION 1.0)

# Configura o C++ para a versão 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Inclui o GLFW direto do código-fonte
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/glfw)

# 2. Informa onde estão os arquivos de cabeçalho (.h)
include_directories(include)

# 3. Define os arquivos de código
set(FONTES
    src/main.cpp
    src/ShaderProgram.cpp
    src/VertexArray.cpp
    src/VertexBuffer.cpp
    src/IndexBuffer.cpp
    src/Renderer.cpp
    src/glad.c
    src/stb_image.cpp
)

# 4. Cria o executável final com o nome do seu projeto
add_executable(opengl_learn_project ${FONTES})

# 5. Linka as bibliotecas usando o nome correto do alvo
target_link_libraries(opengl_learn_project glfw opengl32)

add_custom_command(TARGET opengl_learn_project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/src/res"
    "$<TARGET_FILE_DIR:opengl_learn_project>/res"
)