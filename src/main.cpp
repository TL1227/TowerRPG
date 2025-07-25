#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shaderReader.h"
#include "textures.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

//file scoped variables

static string GameVersion = "0.0.0";

//screen
static int SCREEN_WIDTH = 960;
static int SCREEN_HEIGHT = 540;
static float RATIO = 9.0f / 16.0f;

static bool WireFrameMode = false;
static float MOVE = 1.0f;
static float RIGHTMOVE = 90.0f;
static float RIGHT = 0.0f;
static float UP = 0.0f;
static vec3 CameraPos = vec3(0.0f, 0.0f, 3.0f);
static vec3 CameraFront = vec3(0.0f, 0.0f, -1.0f);
static vec3 CameraUp = vec3(0.0f, 1.0f, 0.0f);
static vec3 YRotAxis = vec3(0.0f, 1.0f, 0.0f);
static float HorRot = -90.0f;

//framerate
static float DeltaTime = 0.0f;
static float LastFrame = 0.0f;
static float FPS = 1.0 / 100.0;

//movement
enum class Direction
{
    None,
    Forwards,
    Backwards
};
static Direction MOVING = Direction::None;
static float MOVESTART = 0.0f;
static float MOVEDIST = 1.0f;
static float MOVESPEED = 2.0f;

enum class Rotation
{
    None,
    Right,
    Left
};
static Rotation ROTATING = Rotation::None;
static float ROTSTART = 0.0f;
static float ROTDIST = 90.0f;
static float ROTSPEED = 70.0f;

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
    else if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
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
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        if (MOVING == Direction::None)
        {
            MOVING = Direction::Forwards;
        }
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        if (MOVING == Direction::None)
        {
            MOVING = Direction::Backwards;
        }
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        if (ROTATING == Rotation::None)
        {
            ROTATING = Rotation::Right;
        }

        /*
        HorRot += 90.0f;

        if (HorRot > 360.0f)
            HorRot = 90.0f;
            */
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        HorRot -= 90.0f;

        if (HorRot < -360.0f)
            HorRot = -90.0f;
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

void RenderCube(int shaderID, float x, float y,  float z)
{
	mat4 model = mat4(1.0f);
	model = translate(model, vec3(x, y, z));
	unsigned int modelLoc = glGetUniformLocation(shaderID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

    //CameraCode 
	CameraFront.x = cos(glm::radians(HorRot));
	CameraFront.z = sin(glm::radians(HorRot));
    mat4 view = lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
	unsigned int viewLoc = glGetUniformLocation(shaderID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));

	mat4 projection;
	projection = perspective(radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MoveChar()
{
    if (MOVING == Direction::Forwards)
    {
		CameraPos += (MOVESPEED * DeltaTime) * CameraFront;
        MOVESTART += (MOVESPEED * DeltaTime);

        if (MOVESTART > MOVEDIST)
        {
            MOVING = Direction::None;
            MOVESTART = 0.0f;
            CameraPos.x = floor(CameraPos.x + 0.5);
            CameraPos.z = floor(CameraPos.z + 0.5);
        }
    }
    else if (MOVING == Direction::Backwards)
    {
		CameraPos -= (MOVESPEED * DeltaTime) * CameraFront;
        MOVESTART += (MOVESPEED * DeltaTime);

        if (MOVESTART > MOVEDIST)
        {
            MOVING = Direction::None;
            MOVESTART = 0.0f;
            CameraPos.x = floor(CameraPos.x + 0.5);
            CameraPos.z = floor(CameraPos.z + 0.5);
        }
    }

    if (ROTATING == Rotation::Right)
    {
        HorRot += ROTSPEED * DeltaTime;
        ROTSTART += ROTSPEED * DeltaTime;

        /*
        if (HorRot > 360.0f)
            HorRot = 90.0f;
            */

        if (ROTSTART > ROTDIST)
        {
            ROTATING = Rotation::None;
            ROTSTART = 0.0f;
        }
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
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int wallTex = LoadTexture("textures\\wall_01.jpg");
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), wallTex);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);

    //TODO: try and abstract this into some kind of block.cpp

	char groundLevel[7][7] =
	{
		 {'w', ' ', ' ', 's', ' ', ' ', 'w'},
		 {'w', ' ', ' ', ' ', ' ', ' ', 'w'},
		 {'w', 'w', 'w', ' ', 'w', 'w', 'w'},
		 {'w', ' ', ' ', ' ', ' ', ' ', 'w'},
		 {'w', ' ', ' ', ' ', ' ', ' ', 'w'},
		 {'w', ' ', ' ', ' ', ' ', ' ', 'w'},
		 {'w', 'w', 'w', 'w', 'w', 'w', 'w'}
	};

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = currentFrame - LastFrame;

        if (DeltaTime >= FPS)
        {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			MoveChar();

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					if (groundLevel[i][j] == 'w')
						RenderCube(shaderProgram, j, 0.0f, i);
				}
			}

			//floor
			for (int i = 0; i < 7; i++)
				for (int j = 0; j < 7; j++)
					RenderCube(shaderProgram, j, -1.0f, i);

			//roof
			for (int i = 0; i < 7; i++)
				for (int j = 0; j < 7; j++)
					RenderCube(shaderProgram, j, 1.0f, i);

			glfwSwapBuffers(window);
            LastFrame = currentFrame;
        }
        else
        {
            glfwWaitEventsTimeout(FPS - DeltaTime);
        }

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
