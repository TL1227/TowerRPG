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
    OffScreenDistance = (float)ScreenWidth * 2.0f;

    Shader textShader{ "shaders\\uivert.shader", "shaders\\uifrag.shader" };
    textShader.use();
	textShader.setMat4("projection", projection);
    Text = { ScreenWidth, ScreenHeight, textShader };

    //TODO: these should probably be in some kind of UI component class
    Shader battleMenuShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    battleMenuShader.use();
	battleMenuShader.setMat4("projection", projection);

    BattleMenu = { "textures\\battlemenu.jpg", battleMenuShader};
    BattleMenu.width = (float)ScreenWidth * 0.65f;
    BattleMenu.height = (float)ScreenHeight * 0.25f;
    BattleMenuOnScreenY = BattleMenu.height / 2.0f; //snap to bottom of screen
    BattleMenuOnScreenX = (float)ScreenWidth / 2.0f; 
    //BattleMenu.x = BattleMenuOnScreenX - OffScreenDistance;
    BattleMenu.x = -BattleMenu.width;
    BattleMenu.y = BattleMenuOnScreenY;
    BattleMenuSlider.duration = preambleDuration;
	BattleMenuSlider.start = BattleMenu.x;
    BattleMenuSlider.end = BattleMenuOnScreenX;

    Shader enemyHpShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    enemyHpShader.use();
	enemyHpShader.setMat4("projection", projection);

    EnemyHealthBar = { "textures\\enemyhealthinner.jpg", enemyHpShader };
    EnemyHealthBarOnScreenX = ScreenWidth / 2.0f;
    EnemyHealthBarOnScreenY = (float)ScreenHeight * 0.93f;
    EnemyHealthBar.y = EnemyHealthBarOnScreenY;
    EnemyHealthBar.width = (float)ScreenWidth * 0.65f;
    EnemyHealthBar.height = (float)ScreenHeight * 0.03f;
    EnemyHealthBar.x = screenWidth + EnemyHealthBar.width;

    //set the enemyHealthstarting pos
    EnemyHealthStartWidth = EnemyHealthBar.width;
    EnemyHealthStartX = EnemyHealthBar.x;

    EnemyHealthBarSlider.duration = preambleDuration;
	EnemyHealthBarSlider.start = EnemyHealthBar.x;
    EnemyHealthBarSlider.end = EnemyHealthBarOnScreenX;
}

void UI::Tick(float deltaTime)
{
    ImGui::Begin("Battle Menu");
    ImGui::SliderFloat("menu", &BattleMenu.x, 0, 2000);
    ImGui::SliderFloat("bar", &EnemyHealthBar.x, 100, 2000);
    ImGui::End();

    if (BattleSystem.GetPhase() == BattlePhase::Preamble)
    {
        Text.Draw("Grrrrr... I'm a Goblin!", 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
    }
    else if (BattleSystem.GetPhase() == BattlePhase::Slide)
    {
        bool slide1complete = Slide(deltaTime, EnemyHealthBar.x, EnemyHealthBarSlider);
        EnemyHealthBar.Draw();

        bool slide2complete = Slide(deltaTime, BattleMenu.x, BattleMenuSlider);
        BattleMenu.Draw();

        std::cout << BattleMenu.y << std::endl;

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

        BattleMenu.x = BattleMenuOnScreenX - OffScreenDistance;

        EnemyHealthBar.x = EnemyHealthBarOnScreenX + OffScreenDistance; //set it up for sliding on to screen
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
