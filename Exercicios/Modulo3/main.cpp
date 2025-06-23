#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// ======= Protótipos =======
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLuint setupShader();
struct Geometry loadGeometry(const char* filepath);

// ======= Estrutura =======
struct Geometry {
    GLuint VAO;
    GLuint vertexCount;
    GLuint textureID = 0;
    string textureFilePath;
};

// ======= Variáveis globais =======
bool rotateX = false, rotateY = false, rotateZ = false;
glm::vec3 translate_vector = { 0.0f, 0.0f, 0.0f };
glm::vec3 scale_vector = { 1.0f, 1.0f, 1.0f };

const GLuint WIDTH = 1000, HEIGHT = 1000;

// ======= Shaders GLSL =======
const GLchar* vertexShaderSource = R"(
#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texc;

uniform mat4 model;

out vec2 texCoord;
out vec4 vertexColor;

void main()
{
    gl_Position = model * vec4(position, 1.0);
    vertexColor = vec4(color, 1.0);
    texCoord = vec2(texc.x, 1.0 - texc.y);
}
)";

const GLchar* fragmentShaderSource = R"(
#version 450 core
in vec2 texCoord;
in vec4 vertexColor;

uniform sampler2D tex_buffer;

out vec4 color;

void main()
{
    color = texture(tex_buffer, texCoord);
}
)";

// ======= MAIN =======
int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - Modulo 3", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }

    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version supported: " << glGetString(GL_VERSION) << endl;

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    GLuint shaderID = setupShader();

    // Atenção: ajuste o path para seu arquivo OBJ conforme a organização da sua pasta
    Geometry g = loadGeometry("assets/Modelos3d/Suzanne.obj");
    if (g.VAO == 0)
        return -1;

    glUseProgram(shaderID);
    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float angle = (float)glfwGetTime();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translate_vector + glm::vec3(0.0f, 0.25f, 0.0f));

        if (rotateX)
            model = glm::rotate(model, angle, glm::vec3(1, 0, 0));
        else if (rotateY)
            model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
        else if (rotateZ)
            model = glm::rotate(model, angle, glm::vec3(0, 0, 1));

        // O código do seu amigo adiciona um scale_vector + (-0.7f, -0.7f, -0.7f) na escala
        model = glm::scale(model, scale_vector + glm::vec3(-0.7f, -0.7f, -0.7f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindTexture(GL_TEXTURE_2D, g.textureID);
        glBindVertexArray(g.VAO);
        glDrawArrays(GL_TRIANGLES, 0, g.vertexCount);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &g.VAO);
    glDeleteTextures(1, &g.textureID);
    glfwTerminate();

    return 0;
}

// ======= CALLBACK DE TECLADO =======
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        rotateX = true, rotateY = false, rotateZ = false;

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
        rotateX = false, rotateY = true, rotateZ = false;

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        rotateX = false, rotateY = false, rotateZ = true;

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        translate_vector += glm::vec3(0.0f, 0.0f, 0.1f);

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        translate_vector += glm::vec3(0.0f, 0.0f, -0.1f);

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        translate_vector += glm::vec3(-0.1f, 0.0f, 0.0f);

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        translate_vector += glm::vec3(0.1f, 0.0f, 0.0f);

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
        translate_vector += glm::vec3(0.0f, 0.1f, 0.0f);

    if (key == GLFW_KEY_J && action == GLFW_PRESS)
        translate_vector += glm::vec3(0.0f, -0.1f, 0.0f);

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        scale_vector += glm::vec3(0.1f);

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        scale_vector += glm::vec3(-0.1f);
}

// ======= SETUP SHADER =======
GLuint setupShader()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// ======= LOAD GEOMETRY =======
Geometry loadGeometry(const char* filepath)
{
    vector<GLfloat> vertices;
    vector<glm::vec3> v;
    vector<glm::vec2> uvs;
    vector<glm::vec3> normals;

    ifstream file(filepath);
    if (!file)
    {
        cerr << "Failed to open file: " << filepath << endl;
        return {};
    }

    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<glm::vec3> temp_vertices;
    vector<glm::vec2> temp_uvs;
    vector<glm::vec3> temp_normals;

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "v")
        {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        else if (type == "vt")
        {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        else if (type == "vn")
        {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (type == "f")
        {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char slash;

            for (int i = 0; i < 3; ++i)
            {
                iss >> vertexIndex[i] >> slash >> uvIndex[i] >> slash >> normalIndex[i];
                vertexIndices.push_back(vertexIndex[i]);
                uvIndices.push_back(uvIndex[i]);
                normalIndices.push_back(normalIndex[i]);
            }
        }
    }

    file.close();

    for (size_t i = 0; i < vertexIndices.size(); ++i)
    {
        unsigned int vi = vertexIndices[i];
        unsigned int ui = uvIndices[i];
        unsigned int ni = normalIndices[i];

        glm::vec3 vert = temp_vertices[vi - 1];
        glm::vec2 uv = temp_uvs[ui - 1];
        glm::vec3 norm = temp_normals[ni - 1];

        v.push_back(vert);
        uvs.push_back(uv);
        normals.push_back(norm);
    }

    vertices.reserve(v.size() * 8);
    for (size_t i = 0; i < v.size(); ++i)
    {
        vertices.insert(vertices.end(), {
            v[i].x, v[i].y, v[i].z,     // posição
            1.0f, 0.0f, 0.0f,           // cor fixa (vermelho)
            uvs[i].x, uvs[i].y          // textura
            });
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    Geometry geometry;
    geometry.VAO = VAO;
    geometry.vertexCount = vertices.size() / 8;

    // Carregamento de textura via arquivo .mtl
    string basePath = string(filepath).substr(0, string(filepath).find_last_of("/"));
    string filenameNoExt = string(filepath).substr(string(filepath).find_last_of("/") + 1);
    filenameNoExt = filenameNoExt.substr(0, filenameNoExt.find_last_of("."));

    string mtlPath = basePath + "/" + filenameNoExt + ".mtl";

    ifstream mtlFile(mtlPath);
    if (!mtlFile)
    {
        cerr << "Failed to open MTL file: " << mtlPath << endl;
        return geometry; // Retorna com o VAO e vertexCount mesmo sem textura
    }

    string texLine, texturePath;
    while (getline(mtlFile, texLine))
    {
        istringstream iss(texLine);
        string keyword;
        iss >> keyword;

        if (keyword == "map_Kd")
        {
            iss >> texturePath;
            break;
        }
    }
    mtlFile.close();

    if (!texturePath.empty())
    {
        string fullTexturePath = basePath + "/" + texturePath;

        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int w, h, ch;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(fullTexturePath.c_str(), &w, &h, &ch, 0);
        if (data)
        {
            GLenum format = (ch == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
            geometry.textureID = texID;
            geometry.textureFilePath = fullTexturePath;
        }
        else
        {
            cerr << "Failed to load texture: " << fullTexturePath << endl;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    return geometry;
}
