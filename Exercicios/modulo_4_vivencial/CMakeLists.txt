# Caminhos do GLAD
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glad/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/common/glad/src/glad.c")

# Caminhos do GLFW compilado
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glfw-3.4/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/common/glfw-3.4/build/src/Debug")  # onde está o glfw3.lib

# Caminho do GLM
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common")

set(stb_image.h_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common")

add_executable(modulo_4_vivencial main.cpp ${GLAD_SRC})

target_include_directories(modulo_4_vivencial PRIVATE
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)


target_link_libraries(modulo_4_vivencial PRIVATE
    "${GLFW_LIB_DIR}/glfw3.lib"
    opengl32  # OpenGL no Windows
)
