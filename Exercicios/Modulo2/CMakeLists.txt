# Caminhos do GLAD
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glad/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/common/glad/src/glad.c")

# Caminhos do GLFW compilado
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glfw-3.4/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/common/glfw-3.4/build/src/Debug")  # onde est� o glfw3.lib

# Caminho do GLM
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common")

add_executable(Modulo2 main.cpp ${GLAD_SRC})

target_include_directories(Modulo2 PRIVATE
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)


target_link_libraries(Modulo2 PRIVATE
    "${GLFW_LIB_DIR}/glfw3.lib"
    opengl32  # OpenGL no Windows
)
