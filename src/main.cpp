#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shaderReader.h"
#include "textures.h"
#include "movement.h"
#include "camera.h"
#include "map.h"
#include "args.h"
#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

//file scoped variables

static string GameVersion = "0.0.0";

Camera camera;

//framerate
static float DeltaTime = 0.0f;
static float LastFrame = 0.0f;
static float FPS = 1.0 / 100.0;

//directories
string DataDir = "data";
string MapPath = "maps";

//movement
Movement CharMove;

//map
Map LevelMap;

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
        static bool WireFrameMode = false;

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

//screen
static int SCREEN_WIDTH = 960;
static int SCREEN_HEIGHT = 540;
GLFWwindow* SetUpGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower RPG", NULL, NULL);

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

int main(int argc, char* argv[])
{
    //init stuff
    G_Args.Parse(argc, argv);
    GLFWwindow* window = SetUpGlfw();
    if (window == NULL) return -1;
    if (!SetUpGlad()) return -1;

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

    Model wall{ vertices, shaderProgram };

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    //Load map
    if (G_Args.MapPath.empty()) 
        LevelMap.Load(DataDir + "\\map.txt");
    else 
        LevelMap.Load(G_Args.MapPath);

    int rowSize = LevelMap.Data.size();
    int columnSize = LevelMap.Data[0].size();

    //set camera at 's' character
    for (int i = 0; i < rowSize; i++)
	{
		for (int j = 0; j < columnSize; j++)
		{
			if (LevelMap.Data[i][j] == 's')
			{
				camera.CameraPos.x = j;
				camera.CameraPos.z = i;
                break;
			}
		}
	}

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);

    int frames = 0;
    float LastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        frames++;

        float currentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = currentFrame - LastFrame;

		if (currentFrame - LastTime >= 1.0)
		{
		    std::cout << "FPS: " << frames << std::endl;
		    std::cout << "LastTime: " << LastTime << std::endl;
            frames = 0;
            LastTime += 1.0;
		}

        if (DeltaTime >= FPS)
        {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
                CharMove.SetMoveAction(MoveAction::TurnRight);
            else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
                CharMove.SetMoveAction(MoveAction::TurnLeft);
            else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT))
                CharMove.SetMoveAction(MoveAction::Right);
            else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT))
                CharMove.SetMoveAction(MoveAction::Left);
            else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
                CharMove.SetMoveAction(MoveAction::Forwards);
            else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
                CharMove.SetMoveAction(MoveAction::Backwards);

			CharMove.MoveChar(camera, DeltaTime);

            camera.UpdateCameraRotation();

            mat4 view = camera.GetView();

			for (int i = 0; i < rowSize; i++)
			{
				for (int j = 0; j < columnSize; j++)
				{
                    if (LevelMap.Data[i][j] == '#')
                    {
						//glm::mat4 model = glm::mat4(1.0f);
						mat4 model = glm::translate(mat4(1.0f), glm::vec3(j, 0.0f, i));
                        wall.Draw(model, view, projection);
                    }
				}
			}

            /*
			//floor
			for (int i = 0; i < rowSize; i++)
				for (int j = 0; j < columnSize; j++)
                        wall.Draw(camera, glm::vec3(j, 0.0f, i));

			//roof
			for (int i = 0; i < rowSize; i++)
				for (int j = 0; j < columnSize; j++)
                        wall.Draw(camera, glm::vec3(j, 0.0f, i));
                        */

			glfwSwapBuffers(window);
            LastFrame = currentFrame;

            //TODO: put this in some kind of ISystem vector so it only gets checked if we're in live edit mode
            if (G_Args.IsLiveEdit)
            {
				if (LevelMap.HasChanged())
					LevelMap.Load();
            }
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
