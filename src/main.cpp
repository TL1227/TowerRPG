#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shader.h"
#include "textures.h"
#include "movement.h"
#include "camera.h"
#include "map.h"
#include "args.h"
#include "model.h"
#include "ui.h"
#include "tile.h"
#include "enemy.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;
using namespace glm;

//file scoped variables

static string GameVersion = "0.0.0";


//framerate
static float DeltaTime = 0.0f;
static float LastFrame = 0.0f;
static float FPS = 1.0 / 100.0;

//directories
string DataDir = "data";
string MapPath = "maps";


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

void ConsoleSplashMessage()
{
    cout << "Tower RPG" << endl;
    cout << "by T-Level Games" << endl;
    cout << "Version" << GameVersion << endl;
}

int main(int argc, char* argv[])
{
    G_Args.Parse(argc, argv);
    GLFWwindow* window = SetUpGlfw();
    if (window == NULL) return -1;
    if (!SetUpGlad()) return -1;

    ConsoleSplashMessage();

	glClearColor(0.7f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
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

    Movement CharMove{ LevelMap, Camera };

    //build shaders
    Shader assetShader{ "shaders\\vert.shader", "shaders\\frag.shader" };
    Shader uiShader{ "shaders\\uivert.shader", "shaders\\uifrag.shader" };
    Shader enemyShader{ "shaders\\enemyvert.shader", "shaders\\enemyfrag.shader" };

    //perspective projection
	mat4 projection = perspective(radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
    assetShader.use();
	assetShader.setMat4("projection", projection);
	enemyShader.use();
	enemyShader.setMat4("projection", projection);

    //orthogonal projection
    projection = ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    uiShader.use();
	uiShader.setMat4("projection", projection);

    //ui init
    UI ui {SCREEN_WIDTH, SCREEN_HEIGHT};
    ui.InitUi();

    Enemy enemy;
    CharMove.Enemy = &enemy;

    int frames = 0;
    float LastTime = glfwGetTime();
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
            else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE))
            {
                CharMove.EndBattle();
            }

			CharMove.MoveChar(DeltaTime);

            //TODO: This could probably get called in MoveChar itself 
            Camera.UpdateCameraRotation();

            assetShader.use();

            mat4 view = Camera.GetView();
			assetShader.setMat4("view", view);

            for (auto& tile : LevelMap.Tiles)
            {
				mat4 model = translate(mat4(1.0f), tile.Position);
				assetShader.setMat4("model", model);

                if (tile.Model)
                {
					tile.Model->Draw(assetShader);
                }
            }

			//floor
			for (int i = 0; i < LevelMap.Data.size(); i++)
				for (int j = 0; j < LevelMap.Data[0].size(); j++)
				{
					mat4 model = translate(mat4(1.0f), vec3(j, -1.0f, i));
					assetShader.setMat4("model", model);
					LevelMap.WallModel.Draw(assetShader);
				}

			//roof
            /*
			for (int i = 0; i < rowSize; i++)
				for (int j = 0; j < colSize; j++)
				{
					mat4 model = translate(mat4(1.0f), vec3(j,  1.0f, i));
					assetShader.setMat4("model", model);
					LevelMap.WallModel.Draw(assetShader);
				}
                */

            //UI DRAWING
            if (CharMove.IsStill())
            {
                if (CharMove.FrontTile)
                {
                    if (!CharMove.FrontTile->InteractiveText.empty())
                    {
                        ui.DrawText(uiShader, CharMove.FrontTile->InteractiveText, 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
                    }
                }
            }

            //Enemy
            if (CharMove.WeBattleNow)
            {
				enemyShader.use();
				mat4 enemymodel = translate(mat4(1.0f), enemy.Position);
				enemymodel = rotate(enemymodel, radians(CharMove.GetDirection() + 90), vec3(0.0f, 1.0f, 0.0f));
				enemyShader.setMat4("model", enemymodel);
				enemyShader.setMat4("view", view);
				enemyShader.setFloat("alpha", (CharMove.DistanceMoved == 0) ? 1.0f : CharMove.DistanceMoved);
				enemy.Draw();
            }

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
