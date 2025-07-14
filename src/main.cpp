#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shaderReader.h"
#include "textures.h"

using namespace std;

//file scoped variables
static string GameVersion = "0.0.0";
static int SCREEN_WIDTH = 960;
static int SCREEN_HEIGHT = 540;
static bool WireFrameMode = false;

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
    {
        if (WireFrameMode)
        {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            WireFrameMode = false;
        }
        else
        {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            WireFrameMode = true;
        }
    }
}

GLFWwindow* SetUpGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    //openglcontext
    glfwMakeContextCurrent(window);

    //callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    //inputmode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    return window;
}

bool SetUpGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void ConsoleSplashMessage()
{
    cout << "Tower RPG" << endl;
    cout << "by T-Level Games" << endl;
    cout << "Version" << GameVersion << endl;
}

void ShowCompileErrors(unsigned int shader)
{
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void ShowLinkingErrors(unsigned int shaderProgram)
{
    char infoLog[512];
    int success;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

int main()
{
    GLFWwindow* window = SetUpGlfw();

    if (window == NULL)
        return -1;

    if (!SetUpGlad()) 
        return -1;

    ConsoleSplashMessage();

    //compile and link shaders    
    string shaderString = ReadShaderFile("shaders\\vert.shader");
    const char* shaderPointer = shaderString.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderPointer, NULL);
    glCompileShader(vertexShader);
    ShowCompileErrors(vertexShader);

    shaderString = ReadShaderFile("shaders\\frag.shader");
    shaderPointer = shaderString.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderPointer, NULL);
    glCompileShader(fragmentShader);
    ShowCompileErrors(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    ShowLinkingErrors(shaderProgram);

    //clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //x, y, z
    vector<float> vertices = {
        // positions          // colors           // texture coords
       0.5f * 9/16,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f * 9/16, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f * 9/16, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f * 9/16,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int arrayBuffer;
    glGenBuffers(1, &arrayBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int vertBuffer;
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int wallTex = LoadTexture("textures\\wall_01.jpg");

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
