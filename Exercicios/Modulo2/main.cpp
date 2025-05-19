#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

// Vertex shader GLSL
const char* vertexShaderSource = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
)glsl";

// Fragment shader GLSL
const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)glsl";

// cubo com 36 vertices (6 faces * 2 triângulos * 3 vértices), posição + cor
float vertices[] = {
    // face frontal (vermelha)
    -0.5f, -0.5f,  0.5f,  1, 0, 0,
     0.5f, -0.5f,  0.5f,  1, 0, 0,
     0.5f,  0.5f,  0.5f,  1, 0, 0,
     0.5f,  0.5f,  0.5f,  1, 0, 0,
    -0.5f,  0.5f,  0.5f,  1, 0, 0,
    -0.5f, -0.5f,  0.5f,  1, 0, 0,

    // face traseira (verde)
    -0.5f, -0.5f, -0.5f,  0, 1, 0,
     0.5f, -0.5f, -0.5f,  0, 1, 0,
     0.5f,  0.5f, -0.5f,  0, 1, 0,
     0.5f,  0.5f, -0.5f,  0, 1, 0,
    -0.5f,  0.5f, -0.5f,  0, 1, 0,
    -0.5f, -0.5f, -0.5f,  0, 1, 0,

    // face esquerda (azul)
    -0.5f,  0.5f,  0.5f,  0, 0, 1,
    -0.5f,  0.5f, -0.5f,  0, 0, 1,
    -0.5f, -0.5f, -0.5f,  0, 0, 1,
    -0.5f, -0.5f, -0.5f,  0, 0, 1,
    -0.5f, -0.5f,  0.5f,  0, 0, 1,
    -0.5f,  0.5f,  0.5f,  0, 0, 1,

    // face direita (amarela)
     0.5f,  0.5f,  0.5f,  1, 1, 0,
     0.5f,  0.5f, -0.5f,  1, 1, 0,
     0.5f, -0.5f, -0.5f,  1, 1, 0,
     0.5f, -0.5f, -0.5f,  1, 1, 0,
     0.5f, -0.5f,  0.5f,  1, 1, 0,
     0.5f,  0.5f,  0.5f,  1, 1, 0,

     // face superior (ciano)
     -0.5f,  0.5f, -0.5f,  0, 1, 1,
      0.5f,  0.5f, -0.5f,  0, 1, 1,
      0.5f,  0.5f,  0.5f,  0, 1, 1,
      0.5f,  0.5f,  0.5f,  0, 1, 1,
     -0.5f,  0.5f,  0.5f,  0, 1, 1,
     -0.5f,  0.5f, -0.5f,  0, 1, 1,

     // face inferior (magenta)
     -0.5f, -0.5f, -0.5f,  1, 0, 1,
      0.5f, -0.5f, -0.5f,  1, 0, 1,
      0.5f, -0.5f,  0.5f,  1, 0, 1,
      0.5f, -0.5f,  0.5f,  1, 0, 1,
     -0.5f, -0.5f,  0.5f,  1, 0, 1,
     -0.5f, -0.5f, -0.5f,  1, 0, 1,
};

struct CubeInstance {
    glm::vec3 position;
    float scale = 1.0f;
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;
};

// Globals para controle
float moveSpeed = 0.05f;
float scaleSpeed = 0.05f;

CubeInstance mainCube;
std::vector<CubeInstance> cubes;

unsigned int shaderProgram;
unsigned int VAO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    // Movimento do cubo principal
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCube.position.z -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCube.position.z += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCube.position.x -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCube.position.x += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        mainCube.position.y += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        mainCube.position.y -= moveSpeed;

    // Escala uniforme
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        mainCube.scale = std::max(0.1f, mainCube.scale - scaleSpeed);
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        mainCube.scale += scaleSpeed;

    // Rotação
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        mainCube.rotX += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        mainCube.rotY += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        mainCube.rotZ += 1.0f;

    // Instanciar novo cubo com tecla N
    static bool nPressedLastFrame = false;
    bool nPressed = glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS;
    if (nPressed && !nPressedLastFrame)
    {
        CubeInstance newCube = mainCube; // copia posição, escala, rotação atual
        cubes.push_back(newCube);
        std::cout << "Instanciou cubo novo! Total: " << cubes.size() + 1 << "\n";
    }
    nPressedLastFrame = nPressed;
}

unsigned int compileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERRO COMPILANDO SHADER: " << infoLog << std::endl;
    }
    return shader;
}

unsigned int createShaderProgram()
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERRO LINKANDO PROGRAMA: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main()
{
    // Inicializa GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cubo 3D com multiplas instancias", NULL, NULL);
    if (!window)
    {
        std::cerr << "Falha ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializa GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compila shaders
    shaderProgram = createShaderProgram();

    // Setup buffers e arrays
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // posições
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // cores
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Cubo principal inicial
    mainCube.position = glm::vec3(0.0f, 0.0f, 0.0f);
    mainCube.scale = 1.0f;

    // Projeção e view fixas (camera simples)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3));

    glEnable(GL_DEPTH_TEST);

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // set uniforms fixos view e projection
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        int modelLoc = glGetUniformLocation(shaderProgram, "model");

        // Desenhar cubo principal
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, mainCube.position);
        model = glm::rotate(model, glm::radians(mainCube.rotX), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(mainCube.rotY), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(mainCube.rotZ), glm::vec3(0, 0, 1));
        model = glm::scale(model, glm::vec3(mainCube.scale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Desenhar cubos instanciados
        for (const CubeInstance& c : cubes)
        {
            glm::mat4 modelInst = glm::mat4(1.0f);
            modelInst = glm::translate(modelInst, c.position);
            modelInst = glm::rotate(modelInst, glm::radians(c.rotX), glm::vec3(1, 0, 0));
            modelInst = glm::rotate(modelInst, glm::radians(c.rotY), glm::vec3(0, 1, 0));
            modelInst = glm::rotate(modelInst, glm::radians(c.rotZ), glm::vec3(0, 0, 1));
            modelInst = glm::scale(modelInst, glm::vec3(c.scale));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelInst));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
