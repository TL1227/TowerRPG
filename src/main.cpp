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
#include "movement.h"
#include "camera.h"
#include "map.h"
#include "args.h"
#include "model.h"
#include "ui.h"
#include "tile.h"
#include "quad.h"

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
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN) || GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))        {
            Choice = static_cast<BattleChoice>((static_cast<int>(Choice) + 1) % (int)BattleChoice::NUM_CHOICES);
        }
        else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP) || GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))        {
            Choice = static_cast<BattleChoice>((static_cast<int>(Choice) - 1 + (int)BattleChoice::NUM_CHOICES) % (int)BattleChoice::NUM_CHOICES);
        }
    }
}

void Waiting()
{

}

bool SlideUp(float delta, float speed, float& var, float end)
{
    if (var < end)
        var += speed * delta;

    if (var > end)
        var = end;

    return var == end;
}

bool SlideDown(float delta, float speed, float& var, float end)
{
    if (var > end)
        var -= speed * delta;
    
    if (var < end)
        var = end;

    return var == end;
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

    Movement CharMove{ LevelMap, Camera };

    //build shaders
    Shader assetShader{ "shaders\\vert.shader", "shaders\\frag.shader" };
    Shader textShader{ "shaders\\uivert.shader", "shaders\\uifrag.shader" };
    Shader battleMenuBgShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    Shader enemyHpShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
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
    battleMenuBgShader.use();
	battleMenuBgShader.setMat4("projection", projection);
    enemyHpShader.use();
	enemyHpShader.setMat4("projection", projection);

    const float SlideSpeed = SCREEN_HEIGHT * 2.0f;
    const float OffScreenDistance = (float)SCREEN_HEIGHT * 0.5f;

    float BattleMenuQuadEndPosY = (float)SCREEN_HEIGHT * 0.2f;
    Quad battleMenuQuad { "textures\\battlemenu.jpg" };
    battleMenuQuad.x = (float)SCREEN_WIDTH / 2.0f;
    battleMenuQuad.y = BattleMenuQuadEndPosY - OffScreenDistance;
    battleMenuQuad.scalex = (float)SCREEN_WIDTH * 0.65f;
    battleMenuQuad.scaley = (float)SCREEN_HEIGHT * 0.25f;

    float EnemyHpInnerQuadEndPosY = (float)SCREEN_HEIGHT * 0.9f;
    Quad enemyHpInnerQuad { "textures\\enemyhealthinner.jpg" };
    enemyHpInnerQuad.x = SCREEN_WIDTH / 2.0f;
    enemyHpInnerQuad.y = EnemyHpInnerQuadEndPosY + OffScreenDistance; //set it up for sliding on to screen
    enemyHpInnerQuad.scalex = (float)SCREEN_WIDTH * 0.65f;
    enemyHpInnerQuad.scaley = (float)SCREEN_HEIGHT * 0.03f;

    float BattleTextScale = 0.45f * TEXT_UNIT;
    float BattleTextXPercent = 0.19f;
    float BattleTextYPercent = 0.28f;
    float BattleTextX = SCREEN_WIDTH * BattleTextXPercent;
    float BattleTextY = SCREEN_HEIGHT * BattleTextYPercent;
    float BattleMenuLineHeightPercent = 0.058;
    float BattleMenuLineHeight = SCREEN_HEIGHT * BattleMenuLineHeightPercent;

    //ui init
    UI ui {SCREEN_WIDTH, SCREEN_HEIGHT};
    ui.InitUi();

    BattleMessage BattleMessage;
    Enemy enemy;
    CharMove.Enemy = &enemy;
    CharMove.BattleMessage = &BattleMessage;

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
            ImGui::InputFloat("X Percent", &BattleTextXPercent);
            ImGui::InputFloat("Y Percent", &BattleTextYPercent);
            ImGui::InputFloat("LineHeightPercent", &BattleMenuLineHeightPercent);
            ImGui::End();
            */
            BattleTextX = SCREEN_WIDTH * BattleTextXPercent;
            BattleTextY = SCREEN_HEIGHT * BattleTextYPercent;
			BattleMenuLineHeight = SCREEN_HEIGHT * BattleMenuLineHeightPercent;

            if (BattleMessage.CurrentPhase == BattlePhase::End || BattleMessage.CurrentPhase == BattlePhase::Sighting)
            {
				if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
                {
					CharMove.SetMoveAction(MoveAction::TurnRight);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
                {
					CharMove.SetMoveAction(MoveAction::TurnLeft);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT))
                {
					CharMove.SetMoveAction(MoveAction::Right);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT))
                {
					CharMove.SetMoveAction(MoveAction::Left);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN))
                {
					CharMove.SetMoveAction(MoveAction::TurnAround);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
                {
					CharMove.SetMoveAction(MoveAction::Forwards);
                }
				else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
                {
					CharMove.SetMoveAction(MoveAction::Backwards);
                }
            }
            else
            {
                //battle controls
                if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE) || GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ENTER))
                {
                    if (Choice == BattleChoice::Run)
                    {
                        BattleMessage.SetBattlePhase(BattlePhase::End);
						CharMove.EndBattle();
                        enemy.SwitchToCalmTex();
						enemyHpInnerQuad.y = EnemyHpInnerQuadEndPosY + OffScreenDistance; //set it up for sliding on to screen
						battleMenuQuad.y = BattleMenuQuadEndPosY - OffScreenDistance;
                    }
                }
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

			//floor TODO: create a floor tile and add them to the towertool map maker
			for (int i = 0; i < LevelMap.Data.size(); i++)
				for (int j = 0; j < LevelMap.Data[0].size(); j++)
				{
					mat4 model = translate(mat4(1.0f), vec3(j, -1.0f, i));
					assetShader.setMat4("model", model);
					LevelMap.WallModel.Draw(assetShader);
				}

            //Enemy Drawing
            if (BattleMessage.CurrentPhase != BattlePhase::End)
            {
				enemyShader.use();
				mat4 enemymodel = translate(mat4(1.0f), enemy.Position);
				enemymodel = rotate(enemymodel, radians(enemy.PlayerDirection + 90), vec3(0.0f, 1.0f, 0.0f));
                enemymodel = scale(enemymodel, vec3(0.8f, 0.8f, 0.0f));
				enemyShader.setMat4("model", enemymodel);
				enemyShader.setMat4("view", view);
				enemyShader.setFloat("alpha", (CharMove.DistanceMoved == 0) ? 1.0f : CharMove.DistanceMoved); //TODO: animate this fade without using CharMove.DistanceMoved
				enemy.Draw();
            }

            //UI DRAWING
            if (CharMove.IsStill())
            {
                if (CharMove.FrontTile)
                {
                    if (!CharMove.FrontTile->InteractiveText.empty())
                    {
                        ui.DrawText(textShader, CharMove.FrontTile->InteractiveText, 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
                    }
                }
                else if (BattleMessage.CurrentPhase == BattlePhase::Preamble)
                {
                    double timepassed = glfwGetTime() -  BattleMessage.PreambleStartTime;
                    if (timepassed > BattleMessage.PreambleLength)
                    {
                        BattleMessage.SetBattlePhase(BattlePhase::Snapin);
                    }
                    else
                    {
                        ui.DrawText(textShader, "Grrrrr... I'm a Goblin!", 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
                    }
                }
                else if (BattleMessage.CurrentPhase == BattlePhase::Snapin) //battle starts propa!
                {
                    bool slide1complete = SlideDown(DeltaTime, SlideSpeed, enemyHpInnerQuad.y, EnemyHpInnerQuadEndPosY);
                    enemyHpInnerQuad.Draw(enemyHpShader);

                    bool slide2complete = SlideUp(DeltaTime, SlideSpeed, battleMenuQuad.y, BattleMenuQuadEndPosY);
                    battleMenuQuad.Draw(battleMenuBgShader);

                    cout << slide1complete << ' ' << slide2complete << endl;

                    if (slide1complete && slide2complete)
                    {
                        BattleMessage.SetBattlePhase(BattlePhase::Start);
                        enemy.SwitchToAttackTex();
                    }
                }
                else if (BattleMessage.CurrentPhase == BattlePhase::Start) //battle starts propa!
                {
                    CurrentMode = Mode::TowerBattle;

                    enemyHpInnerQuad.Draw(enemyHpShader);
                    battleMenuQuad.Draw(battleMenuBgShader);

					vector<string> bmenu = { "Attack", "Skill", "Item", "Run" };
					for (size_t i = 0; i < bmenu.size(); i++)
					{
						if ((int)Choice == i)
							ui.DrawText(textShader, bmenu[i], BattleTextX, BattleTextY - (i * BattleMenuLineHeight), BattleTextScale, glm::vec3(1.0, 1.0, 1.0));
						else
							ui.DrawText(textShader, bmenu[i], BattleTextX, BattleTextY - (i * BattleMenuLineHeight), BattleTextScale, glm::vec3(0.3, 0.3, 0.3));
					}
                }
            }

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
