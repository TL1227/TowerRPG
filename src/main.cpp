#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <vector>

#include "shader.h"
#include "movementsystem.h"
#include "camera.h"
#include "map.h"
#include "args.h"
#include "model.h"
#include "text.h"
#include "tile.h"
#include "quad.h"
#include "audio.h"
#include "ui.h"

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

enum class Mode
{
    TowerExplore,
    TowerBattle
};

enum class BattleChoice
{
    Attack,
    Skill,
    Item,
    Run,
    NUM_CHOICES
};

BattleChoice Choice = BattleChoice::Attack;
Mode CurrentMode = Mode::TowerExplore;

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

    if (CurrentMode == Mode::TowerBattle)
    {
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN) || GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
        {
            Choice = static_cast<BattleChoice>((static_cast<int>(Choice) + 1) % (int)BattleChoice::NUM_CHOICES);
        }
        else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP) || GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
        {
            Choice = static_cast<BattleChoice>((static_cast<int>(Choice) - 1 + (int)BattleChoice::NUM_CHOICES) % (int)BattleChoice::NUM_CHOICES);
        }
    }
}

bool SlideUp(float delta, float speed, float& var, float end)
{
    if (var < end)
        var += speed * delta;

    if (var > end)
        var = end;    return var == end;
}

bool SlideDown(float delta, float speed, float& var, float end)
{
    if (var > end)
        var -= speed * delta;
    
    if (var < end)
        var = end;

    return var == end;
}

struct Slider {
    float start;
    float end;
    float elapsed = 0.0f;
    float duration = 2.6f;   // seconds
};

bool Slide(float delta, float& var, Slider& s)
{
    s.elapsed += delta;

    float t = s.elapsed / s.duration;
    if (t > 1.0f) t = 1.0f;

    var = s.start + (s.end - s.start) * t;

    return t >= 1.0f;
}

//TODO: Look into different ways of scaling text
int TEXT_UNIT;

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
        TEXT_UNIT = 2;

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower RPG", monitor, NULL);
    }
    else
    {
		SCREEN_WIDTH = 960;
		SCREEN_HEIGHT = 540;
        TEXT_UNIT = 1;
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
    Shader assetShader{ "shaders\\vert.shader", "shaders\\frag.shader" };
    Shader textShader{ "shaders\\uivert.shader", "shaders\\uifrag.shader" };
    Shader enemyShader{ "shaders\\enemyvert.shader", "shaders\\enemyfrag.shader" };

    //perspective projection
	mat4 projection = perspective(radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
    assetShader.use();
	assetShader.setMat4("projection", projection);
	enemyShader.use();
	enemyShader.setMat4("projection", projection);

    //orthogonal projection
    projection = ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    textShader.use();
	textShader.setMat4("projection", projection);

    UI ui;

    const float SlideSpeed = SCREEN_HEIGHT * 2.0f;
    float OffScreenDistance = (float)SCREEN_HEIGHT * 0.5f;


    float BattleTextScale = 0.45f * TEXT_UNIT;
    float BattleTextXPercent = 0.19f;
    float BattleTextYPercent = 0.28f;
    float BattleTextX = SCREEN_WIDTH * BattleTextXPercent;
    float BattleTextY = SCREEN_HEIGHT * BattleTextYPercent;
    float BattleMenuLineHeightPercent = 0.058;
    float BattleMenuLineHeight = SCREEN_HEIGHT * BattleMenuLineHeightPercent;

    //ui init
    Text Text {SCREEN_WIDTH, SCREEN_HEIGHT};

    Audio audio;
    BattleSystem BattleSystem;
    BattleSystem.Audio = &audio;
    BattleSystem.PreambleLength = audio.PreBattleBgmLength * 0.8;


    Slider BattleMenuSlider;
    BattleMenuSlider.duration = audio.PreBattleBgmLength * 0.2;
	BattleMenuSlider.start = battleMenuQuad.y;
    BattleMenuSlider.end = BattleMenuQuadEndPosY + 10;

    Enemy Enemy{enemyShader};
    MovementSystem.Enemy = &Enemy;
    MovementSystem.BattleSystem = &BattleSystem;

    //subscribe to Battle events
    BattleEvent Be;
    Be.AddListener(MovementSystem);
    Be.AddListener(Enemy);
    Be.AddListener(audio);
    BattleSystem.BattleEvent = &Be;

    //subscribe to Movement events
    MovementEvent Me;
    Me.AddListener(Enemy);
    MovementSystem.Event = &Me;


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    float menuTextx = 50;
    float menuTexty = 80;

    int battleChoice = 0;
    bool pressed = false;

    int frames = 0;
    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;
    float FPS = 1.0f / 100.0f;
    float LastTime = (float)glfwGetTime();
    float StartBattleTime;

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

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            /*
            ImGui::Begin("Battle Text");
            ImGui::InputFloat("BMQ y", &battleMenuQuad.y);
            ImGui::End();
            */

            BattleTextX = SCREEN_WIDTH * BattleTextXPercent;
            BattleTextY = SCREEN_HEIGHT * BattleTextYPercent;
			BattleMenuLineHeight = SCREEN_HEIGHT * BattleMenuLineHeightPercent;

            if (BattleSystem.GetPhase() == BattlePhase::End || BattleSystem.GetPhase() == BattlePhase::Sighting)
            {
				if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
                {
					MovementSystem.ProcessMoveAction(MoveAction::TurnRight);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
                {
					MovementSystem.ProcessMoveAction(MoveAction::TurnLeft);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT))
                {
					MovementSystem.ProcessMoveAction(MoveAction::Right);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT))
                {
					MovementSystem.ProcessMoveAction(MoveAction::Left);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN))
                {
					MovementSystem.ProcessMoveAction(MoveAction::TurnAround);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
                {
					MovementSystem.ProcessMoveAction(MoveAction::Forwards);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
                {
					MovementSystem.ProcessMoveAction(MoveAction::Backwards);
                }
            }
            else
            {
                //battle controls
                if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE) || GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ENTER))
                {
                    if (Choice == BattleChoice::Run)
                    {
                        BattleSystem.SetBattlePhase(BattlePhase::End);
                        Enemy.SwitchToCalmTex();
						enemyHpInnerQuad.y = EnemyHpInnerQuadEndPosY + OffScreenDistance; //set it up for sliding on to screen
						battleMenuQuad.y = BattleMenuQuadEndPosY - OffScreenDistance;
                    }
                }
            }

			MovementSystem.Tick(DeltaTime);

            //TODO: Maybe call this after direction change
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

			//floor TODO: create a floor tile and add them to the towertool map maker
			for (int i = 0; i < LevelMap.Data.size(); i++)
				for (int j = 0; j < LevelMap.Data[0].size(); j++)
				{
					mat4 model = translate(mat4(1.0f), vec3(j, -1.0f, i));
					assetShader.setMat4("model", model);
					LevelMap.WallModel.Draw(assetShader);
				}

			Enemy.Tick(view);

            //UI DRAWING
            //if (MovementSystem.IsStill())
            //{
                if (BattleSystem.GetPhase() == BattlePhase::Preamble)
                {
                    double timepassed = glfwGetTime() -  BattleSystem.PreambleStartTime;

                    //TODO: This is roughly how long the sliding takes, we need to calculate this dynamically somehow
                    if (timepassed >= BattleSystem.PreambleLength) 
                    {
                        BattleSystem.SetBattlePhase(BattlePhase::Slide);
                    }
                    else
                    {
                        Text.Draw(textShader, "Grrrrr... I'm a Goblin!", 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
                    }
                }
                else if (BattleSystem.GetPhase() == BattlePhase::Slide) //battle starts propa!
                {
                    bool slide1complete = Slide(DeltaTime, enemyHpInnerQuad.y, EnemyHealthSlider);
                    enemyHpInnerQuad.Draw(enemyHpShader);

                    bool slide2complete = Slide(DeltaTime, battleMenuQuad.y, BattleMenuSlider);
                    battleMenuQuad.Draw(battleMenuBgShader);

                    if (slide1complete && slide2complete)
                    {
                        BattleSystem.SetBattlePhase(BattlePhase::Snap);
                        Enemy.SwitchToAttackTex();
                        EnemyHealthSlider.elapsed = 0.0f; //Make a reset slider func
                        BattleMenuSlider.elapsed = 0.0f;
                    }
                }
                else if (BattleSystem.GetPhase() == BattlePhase::Snap)
                {
                    bool slide1complete = SlideUp(DeltaTime, SlideSpeed / 5, enemyHpInnerQuad.y, EnemyHpInnerQuadEndPosY);
                    enemyHpInnerQuad.Draw(enemyHpShader);

                    bool slide2complete = SlideDown(DeltaTime, SlideSpeed / 5, battleMenuQuad.y, BattleMenuQuadEndPosY);
                    battleMenuQuad.Draw(battleMenuBgShader);

                    if (slide1complete && slide2complete)
                    {
                        BattleSystem.SetBattlePhase(BattlePhase::Start);
                    }
                }
                else if (BattleSystem.GetPhase() == BattlePhase::Start) //battle starts propa!
                {
                    CurrentMode = Mode::TowerBattle;

                    enemyHpInnerQuad.Draw(enemyHpShader);
                    battleMenuQuad.Draw(battleMenuBgShader);

					vector<string> bmenu = { "Attack", "Skill", "Item", "Run" };
					for (size_t i = 0; i < bmenu.size(); i++)
					{
						if ((int)Choice == i)
							Text.Draw(textShader, bmenu[i], BattleTextX, BattleTextY - (i * BattleMenuLineHeight), BattleTextScale, glm::vec3(1.0, 1.0, 1.0));
						else
							Text.Draw(textShader, bmenu[i], BattleTextX, BattleTextY - (i * BattleMenuLineHeight), BattleTextScale, glm::vec3(0.3, 0.3, 0.3));
					}
                }
            //}

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

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
