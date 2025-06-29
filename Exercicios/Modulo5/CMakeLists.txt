# Caminhos do GLAD
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glad/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/common/glad/src/glad.c")

# Caminhos do GLFW compilado
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glfw-3.4/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/common/glfw-3.4/build/src/Debug")  # onde est� o glfw3.lib

# Caminho do GLM
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common")

add_executable(Modulo5 main.cpp ${GLAD_SRC} "Camera.h" "Camera.cpp")

target_include_directories(Modulo5 PRIVATE
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

target_link_libraries(Modulo5 PRIVATE
    "${GLFW_LIB_DIR}/glfw3.lib"
    opengl32  # OpenGL no Windows
)

add_custom_command(TARGET Modulo5 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:Modulo5>/assets"
)
