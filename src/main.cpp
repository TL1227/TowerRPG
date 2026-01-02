#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "shader.h"
#include "movementsystem.h"
#include "camera.h"
#include "map.h"
#include "args.h"
#include "audio.h"
#include "ui.h"
#include "input.h"

using namespace std;
using namespace glm;

//file scoped variables
static string GameVersion = "0.0.0";

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

//directories
string DataDir = "data";
string MapPath = "maps";

bool fullscreen = false;
//bool fullscreen = true;

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
GLFWwindow* SetUpGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = nullptr;
    if (fullscreen)
    {
		SCREEN_WIDTH = 1920;
		SCREEN_HEIGHT = 1080;

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower RPG", monitor, NULL);
    }
    else
    {
		SCREEN_WIDTH = 960;
		SCREEN_HEIGHT = 540;
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower RPG", NULL, NULL);
    }

    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
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
        cout << "Failed to initialize GLAD" << endl;
        return false;
    }

    return true;
}

void ImGuiSetup(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void ImGuiStartLoop()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiEndLoop()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(int argc, char* argv[])
{
    G_Args.Parse(argc, argv);

    GLFWwindow* window = SetUpGlfw();
    if (window == NULL) return -1;
    if (!SetUpGlad()) return -1;

    ImGuiSetup(window);

    cout << "Tower RPG" << endl;
    cout << "by T-Level Games" << endl;
    cout << "Version" << GameVersion << endl;

    glClearColor(0.7f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE); //NOTE: This is off because the enemy sprite currently has the wrong winding order and gets culled
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Map LevelMap;
    Camera Camera;

    //Load map
    if (G_Args.MapPath.empty()) 
        LevelMap.Load(DataDir + "\\map.txt");
    else 
        LevelMap.Load(G_Args.MapPath);

    Camera.CameraPos = LevelMap.PlayerStartPos;

    MovementSystem MovementSystem{ LevelMap, Camera };

    //build shaders
    Shader enemyShader{ "shaders\\enemyvert.shader", "shaders\\enemyfrag.shader" };

    //TODO: move this into some global space everything can access?
    //perspective projection
    mat4 projection = perspective(radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
    enemyShader.use();
    enemyShader.setMat4("projection", projection);

    Audio audio;
    BattleSystem BattleSystem;
    BattleSystem.PreambleLength = audio.PreBattleBgmLength * 0.8;

    float preambleLength = (float)audio.PreBattleBgmLength * 0.2f; 
    UI Ui { preambleLength, BattleSystem, SCREEN_HEIGHT, SCREEN_WIDTH };

    Enemy Enemy{enemyShader};
    MovementSystem.Enemy = &Enemy;
    MovementSystem.BattleSystem = &BattleSystem;

    BattleSystem.Enemy = &Enemy;

    //Input
    InputEvent Ie;
    Ie.AddListener(MovementSystem);
    Ie.AddListener(BattleSystem);
    Ie.AddListener(Ui);
    Input Input{ window, &Ie, &BattleSystem };

    //subscribe to Battle events
    BattleEvent Be;
    Be.AddListener(MovementSystem);
    Be.AddListener(Enemy);
    Be.AddListener(audio);
    Be.AddListener(Ui);
    BattleSystem.BattleEvent = &Be;

    //subscribe to Movement events
    MovementEvent Me;
    Me.AddListener(Enemy);
    MovementSystem.Event = &Me;


    float menuTextx = 50;
    float menuTexty = 80;

    int battleChoice = 0;
    bool pressed = false;

    int frames = 0;
    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;
    float FPS = 1.0f / 100.0f;
    float LastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        frames++;

        float currentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = currentFrame - LastFrame;

        if (currentFrame - LastTime >= 1.0)
        {
            //cout << "FPS: " << frames << endl;
            //cout << "LastTime: " << LastTime << endl;
            frames = 0;
            LastTime += 1.0;
        }

        if (DeltaTime >= FPS)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGuiStartLoop();

            Input.Read();
            MovementSystem.Tick(DeltaTime);

            Camera.UpdateCameraRotation();
            mat4 view = Camera.GetView();

            LevelMap.Draw(view);
            Enemy.Tick(DeltaTime, view);
            Ui.Tick(DeltaTime);
            BattleSystem.Tick(DeltaTime);

            //TODO: come back to this once we've cleaned up main
            /*
            if (MovementSystem.FrontTile)
            {
                if (!MovementSystem.FrontTile->InteractiveText.empty())
                {
                    Text.DrawText(textShader, MovementSystem.FrontTile->InteractiveText, 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
                }
            }
            */

            ImGuiEndLoop();
            
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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
