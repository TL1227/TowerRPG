#include "ui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui.h"

static float TopMarg = 58;
static float LeftMarg = 20;

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

    BattleMenuOnScreenY = (float)ScreenHeight * 0.2f;
    BattleMenu = { "textures\\battlemenu.jpg", battleMenuShader};
    BattleMenu.x = (float)ScreenWidth / 2.0f;
    BattleMenu.y = BattleMenuOnScreenY - OffScreenDistance;
    BattleMenu.scalex = (float)ScreenWidth * 0.65f;
    BattleMenu.scaley = (float)ScreenHeight * 0.25f;
    BattleMenuSlider.duration = preambleDuration;
	BattleMenuSlider.start = BattleMenu.y;
    BattleMenuSlider.end = BattleMenuOnScreenY;

    Shader enemyHpShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    enemyHpShader.use();
	enemyHpShader.setMat4("projection", projection);

    EnemyHealthBarOnScreenY = (float)ScreenHeight * 0.93f;
    EnemyHealthBar = { "textures\\enemyhealthinner.jpg", enemyHpShader };
    EnemyHealthBar.x = ScreenWidth / 2.0f;
    EnemyHealthBar.y = EnemyHealthBarOnScreenY + OffScreenDistance;
    EnemyHealthBar.scalex = (float)ScreenWidth * 0.65f;
    EnemyHealthBar.scaley = (float)ScreenHeight * 0.03f;

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

		for (size_t i = 0; i < bmenu.size(); i++)
		{
            glm::vec3 tColour = (i == BattleMenuChoice) ? glm::vec3{ 1.0, 1.0, 1.0 } : glm::vec3{ 0.5, 0.5, 0.5 };
            float textX = BattleMenu.Left() + (LeftMarg * TextScale);
            float textY = BattleMenu.Top() - (TopMarg * TextScale * (i + 1));

            Text.Draw(bmenu[i], textX, textY, TextScale, tColour);
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
    }
}

void UI::OnMenuActionButtonPress(MenuAction button)
{
    switch (button)
    {
	case MenuAction::Confirm:
		if (BattleMenuChoice == 3) //TODO: This needs to be an enum or something
			BattleSystem.SetBattlePhase(BattlePhase::End);

            std::cout << BattleMenuChoice << std::endl;
		break;
	case MenuAction::Cancel:
		break;
	case MenuAction::Up:
        if (--BattleMenuChoice < 0)
			BattleMenuChoice = (int)bmenu.size() - 1;

            std::cout << BattleMenuChoice << std::endl;
		break;
	case MenuAction::Down:
        if (++BattleMenuChoice >= bmenu.size())
			BattleMenuChoice = 0;

            std::cout << BattleMenuChoice << std::endl;
		break;
	case MenuAction::Right:
		break;
	case MenuAction::Left:
		break;
    }
}
