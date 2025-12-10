#include "ui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui.h"

static float TopMarg = 58;
static float LeftMarg = 20;
static float multi = 0.2f;
static float EnemyHealthStartWidth = 0.0f;
static float EnemyHealthStartX = 0.0f;

UI::UI(float preambleDuration, ::BattleSystem& battleSystem, int screenHeight, int screenWidth)
    : BattleSystem{ battleSystem }
    , ScreenHeight{ screenHeight }
    , ScreenWidth{ screenWidth }
{
    //This is so our text can scale with our screensize
    //I've picked 540 because it's the screen size I'm usually using when developing (half of 1080)
    ScreenScale = (float)ScreenWidth / 960;
    TextScale = ScreenScale * 0.5f;

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(ScreenWidth), 0.0f, static_cast<float>(ScreenHeight));
    OffScreenDistance = (float)ScreenHeight * 0.5f;

    Shader textShader{ "shaders\\uivert.shader", "shaders\\uifrag.shader" };
    textShader.use();
	textShader.setMat4("projection", projection);
    Text = { ScreenWidth, ScreenHeight, textShader };

    //TODO: these should probably be in some kind of UI component class
    Shader battleMenuShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    battleMenuShader.use();
	battleMenuShader.setMat4("projection", projection);

    //BattleMenuOnScreenY = (float)ScreenHeight * 0.07f;

    BattleMenu = { "textures\\battlemenu.jpg", battleMenuShader};
    BattleMenu.width = (float)ScreenWidth * 0.65f;
    BattleMenu.height = (float)ScreenHeight * 0.25f;
    BattleMenu.x = (float)ScreenWidth / 2.0f;
    BattleMenuOnScreenY = BattleMenu.height / 2.0f; //snap to bottom of screen
    BattleMenuSlider.duration = preambleDuration;
	BattleMenuSlider.start = BattleMenu.y;
    BattleMenuSlider.end = BattleMenuOnScreenY;

    //Set the menu to be "off screen"
    BattleMenu.y = BattleMenuOnScreenY - OffScreenDistance;

    Shader enemyHpShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    enemyHpShader.use();
	enemyHpShader.setMat4("projection", projection);

    EnemyHealthBarOnScreenY = (float)ScreenHeight * 0.93f;
    EnemyHealthBar = { "textures\\enemyhealthinner.jpg", enemyHpShader };
    EnemyHealthBar.x = ScreenWidth / 2.0f;
    EnemyHealthBar.y = EnemyHealthBarOnScreenY + OffScreenDistance;
    EnemyHealthBar.width = (float)ScreenWidth * 0.65f;
    EnemyHealthBar.height = (float)ScreenHeight * 0.03f;

    //set the enemyHealthstarting pos
    EnemyHealthStartWidth = EnemyHealthBar.width;
    EnemyHealthStartX = EnemyHealthBar.x;

    EnemyHealthBarSlider.duration = preambleDuration;
	EnemyHealthBarSlider.start = EnemyHealthBar.y;
    EnemyHealthBarSlider.end = EnemyHealthBarOnScreenY;
}

void UI::Tick(float deltaTime)
{
    if (BattleSystem.GetPhase() == BattlePhase::Preamble)
    {
        Text.Draw("Grrrrr... I'm a Goblin!", 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
    }
    else if (BattleSystem.GetPhase() == BattlePhase::Slide)
    {
        bool slide1complete = Slide(deltaTime, EnemyHealthBar.y, EnemyHealthBarSlider);
        EnemyHealthBar.Draw();

        bool slide2complete = Slide(deltaTime, BattleMenu.y, BattleMenuSlider);
        BattleMenu.Draw();

        if (slide1complete && slide2complete)
        {
            BattleSystem.SetBattlePhase(BattlePhase::Start);
            ResetSliders();
        }
    }
    else if (BattleSystem.GetPhase() == BattlePhase::Start)
    {
        BattleMenu.Draw();
        EnemyHealthBar.Draw();

        float top = BattleMenu.Top();
        float right = BattleMenu.Right();
        float bottom = BattleMenu.Bottom();
        float left = BattleMenu.Left();

        ImGui::Begin("Battle Menu");
        ImGui::InputFloat("BattleMenu Y", &BattleMenu.y, 0, 600);
        ImGui::InputFloat("BattleMenu On Screen Y", &BattleMenuOnScreenY, 0, 600);
        ImGui::InputFloat("Top", &top, 0, 600);
        ImGui::InputFloat("Right", &right, 0, 600);
        ImGui::InputFloat("Bottom", &bottom, 0, 600);
        ImGui::InputFloat("Left", &left, 0, 600);
        ImGui::End();

		for (size_t i = 0; i < BattleSystem.BattleMenuChoiceSize; i++)
		{
            glm::vec3 tColour = (i == BattleSystem.BattleMenuChoiceIndex) ? glm::vec3{ 1.0, 1.0, 1.0 } : glm::vec3{ 0.5, 0.5, 0.5 };
            float textX = BattleMenu.Left() + (LeftMarg * TextScale);
            float textY = BattleMenu.Top() - (TopMarg * TextScale * (i + 1));

            Text.Draw(BattleSystem.BattleMenuText[i], textX, textY, TextScale, tColour);
		}
    }
}

bool UI::Slide(float deltaTime, float& var, Slider& s)
{
    s.elapsed += deltaTime;

    float t = s.elapsed / s.duration;
    if (t > 1.0f) t = 1.0f;

    var = s.start + (s.end - s.start) * t;

    return t >= 1.0f;
}

void UI::ResetSliders()
{
    EnemyHealthBarSlider.elapsed = 0.0f;
    BattleMenuSlider.elapsed = 0.0f;
}

void UI::OnBattlePhaseChange(BattlePhase bp)
{
    if (bp == BattlePhase::End)
    {
        ResetSliders();

        BattleMenu.y = BattleMenuOnScreenY - OffScreenDistance;

        EnemyHealthBar.y = EnemyHealthBarOnScreenY + OffScreenDistance; //set it up for sliding on to screen
        EnemyHealthBar.x = EnemyHealthStartX;
        EnemyHealthBar.width = EnemyHealthStartWidth;
    }
}

void UI::OnMenuActionButtonPress(MenuAction button)
{
    //TODO: maybe get rid of this?
}

void UI::OnEnemyDamage(float damagePercent)
{
    float damageDone = EnemyHealthStartWidth * (damagePercent / 100);
    EnemyHealthBar.width -= damageDone;
    EnemyHealthBar.x -= (damageDone / 2);
}
