#include "ui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui.h"
#include "menuaction.h"
#include "text.h"
#include "args.h"

static float TopMarg = 58;
static float MenuTextLeftX;

static bool PlayerTurn = true;

UI::UI(float preambleDuration, ::BattleSystem& systems, ::ScreenSize screenSize)
    : BattleSystem{ systems}
    , ScreenSize{ screenSize }
{
    //This is so our text can scale with our screensize
    ScreenScale = (float)ScreenSize.Width / 960;
    TextScale = ScreenScale * 0.43f;

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(ScreenSize.Width), 0.0f, static_cast<float>(ScreenSize.Height));
    OffScreenDistance = (float)ScreenSize.Width * 2.0f;

    Shader textShader{ "shaders\\uivert.shader", "shaders\\uifrag.shader" };
    textShader.use();
	textShader.setMat4("projection", projection);
    BattleMenuText = { ScreenSize.Width, ScreenSize.Height, textShader };
    CurrentCharNameText = { ScreenSize.Width, ScreenSize.Height, textShader };

    Shader battleMenuShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    battleMenuShader.use();
	battleMenuShader.setMat4("projection", projection);

    BattleMenuQuad = { "textures\\battlemenu.jpg", battleMenuShader};
    BattleMenuQuad.width = (float)ScreenSize.Width * 0.65f;
    BattleMenuQuad.height = (float)ScreenSize.Height * 0.20f;
    BattleMenuQuad.x = -BattleMenuQuad.width;
    BattleMenuOnScreenY = BattleMenuQuad.height / 2.0f; //snap to bottom of screen
    BattleMenuQuad.y = BattleMenuOnScreenY;
    BattleMenuOnScreenX = (float)ScreenSize.Width / 2.0f; 
    BattleMenuSlider.duration = preambleDuration;
	BattleMenuSlider.start = BattleMenuQuad.x;
    BattleMenuSlider.end = BattleMenuOnScreenX;

    //battle menu text
    MenuTextLeftX = BattleMenuOnScreenX - (BattleMenuQuad.width / 4);
    CharNameY = BattleMenuOnScreenY;
    CharNameX = BattleMenuOnScreenX - ((BattleMenuQuad.width / 8) * 3.5);

    //EnemyHP
    Shader enemyHpShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    enemyHpShader.use();
	enemyHpShader.setMat4("projection", projection);

    EnemyHealthBarQuad = { "textures\\enemyhealthinner.jpg", enemyHpShader };
    EnemyHealthBarOnScreenX = ScreenSize.Width / 2.0f;
    EnemyHealthBarOnScreenY = (float)ScreenSize.Height * 0.93f;
    EnemyHealthBarQuad.y = EnemyHealthBarOnScreenY;
    EnemyHealthBarQuad.width = (float)ScreenSize.Width * 0.65f;
    EnemyHealthBarQuad.height = (float)ScreenSize.Height * 0.03f;
    EnemyHealthBarQuad.x = ScreenSize.Width + EnemyHealthBarQuad.width;

    EnemyHealthStartWidth = EnemyHealthBarQuad.width;
    EnemyHealthStartX = EnemyHealthBarQuad.x;

    EnemyHealthBarSlider.duration = preambleDuration;
	EnemyHealthBarSlider.start = EnemyHealthBarQuad.x;
    EnemyHealthBarSlider.end = EnemyHealthBarOnScreenX;

    //PartyHP
    PartyHealthBarQuad = { "textures\\partyhealthbar.jpg", enemyHpShader };
    PartyHealthBarOnScreenX = ScreenSize.Width / 2.0f;
    //PartyHealthBarOnScreenY = (float)ScreenSize.Height * 0.93f;
    PartyHealthBarQuad.width = (float)ScreenSize.Width * 0.65f;
    PartyHealthBarQuad.height = (float)ScreenSize.Height * 0.03f;
    PartyHealthBarQuad.y = BattleMenuQuad.Top() + (PartyHealthBarQuad.height / 2);
    PartyHealthBarQuad.x = -PartyHealthBarQuad.width;

    PartyHealthStartWidth = PartyHealthBarQuad.width;
    PartyHealthStartX = PartyHealthBarQuad.x;

    PartyHealthBarSlider.duration = preambleDuration;
	PartyHealthBarSlider.start = PartyHealthBarQuad.x;
    PartyHealthBarSlider.end = PartyHealthBarOnScreenX;

    CurrentBP = BattleSystem.GetPhase();
}

void UI::Tick(float deltaTime)
{
    if (G_Args.GuiOn)
    {
		ImGui::Begin("UI");
		ImGui::SliderFloat("TextChoicex", &MenuTextLeftX, 0, 1000);
		ImGui::SliderFloat("BattleMenux", &BattleMenuQuad.x, 0, 1000);
		ImGui::InputFloat("ScreenScale", &ScreenScale, 0, 1000);
		ImGui::InputFloat("TextScale", &TextScale, 0, 1000);
		ImGui::End();
    }

    if (CurrentBP == BattlePhase::Preamble)
    {
        BattleMenuText.Draw("Grrrrr... I'm a Goblin!", 0, 200, TextScale, RedColour, TextAlign::Center);
    }
    else if (CurrentBP == BattlePhase::Slide)
    {
        bool slide1complete = Slide(deltaTime, EnemyHealthBarQuad.x, EnemyHealthBarSlider);
        EnemyHealthBarQuad.Draw();

        bool slide2complete = Slide(deltaTime, BattleMenuQuad.x, BattleMenuSlider);
        BattleMenuQuad.Draw();

        bool slide3complete = Slide(deltaTime, PartyHealthBarQuad.x, PartyHealthBarSlider);
        PartyHealthBarQuad.Draw();

        if (slide1complete && slide2complete && slide3complete)
        {
            BattleSystem.SetBattlePhase(BattlePhase::Start);
            ResetSliders();
        }
    }
    else if (CurrentBP == BattlePhase::Start || BattleSystem.GetPhase() == BattlePhase::StartTurn)
    {
        BattleMenuQuad.Draw();
        EnemyHealthBarQuad.Draw();
        PartyHealthBarQuad.Draw();

        BattleMenuText.Draw(CurrentCharName, CharNameX, CharNameY, TextScale, HighlightColour);

        //TODO: stop calculating text pos on every frame
		for (size_t i = 0; i < BattleSystem.BattleMenuSize; i++)
		{
            glm::vec3 tColour = (i == BattleSystem.BattleMenuIndex) ? HighlightColour : NoHighlightColour;
            float textY = BattleMenuQuad.Top() - (TopMarg * TextScale * (i + 1));

            BattleMenuText.Draw(BattleSystem.BattleMenuList[i], MenuTextLeftX, textY, TextScale, tColour);
		}
    }
    else if (CurrentBP == BattlePhase::ChoosingSkill)
    {
        BattleMenuQuad.Draw();
        EnemyHealthBarQuad.Draw();
        PartyHealthBarQuad.Draw();

        BattleMenuText.Draw(CurrentCharName, CharNameX, CharNameY, TextScale, HighlightColour);


		for (size_t i = 0; i < BattleSystem.SkillListSize; i++)
		{
            glm::vec3 tColour = (i == BattleSystem.SkillListIndex) ? HighlightColour : NoHighlightColour;
            float textY = BattleMenuQuad.Top() - (TopMarg * TextScale * (i + 1));

            BattleMenuText.Draw(BattleSystem.SkillList[i].Name, MenuTextLeftX, textY, TextScale, tColour);
		}

    }
    else if (CurrentBP == BattlePhase::ExecuteTurn)
    {
        BattleMenuQuad.Draw();
        EnemyHealthBarQuad.Draw();
        PartyHealthBarQuad.Draw();

        if (DamageMe)
        {
            Shake(deltaTime);
        }

        //TODO: Why isn't this centering properly (>_<)
        if (PlayerTurn)
			BattleMenuText.Draw(CurrentTurnText, 0, BattleMenuOnScreenY, TextScale, HighlightColour, TextAlign::Center);
        else
			BattleMenuText.Draw(CurrentTurnText, 0, 200, TextScale, RedColour, TextAlign::Center);
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

void UI::Shake(float delta)
{
    static float timePerFrame = 0.02f;
    static float shakeAmount = 4.0f;
    static int shakeCount = 0;
    static float elapsed = timePerFrame;

    elapsed += delta;

    if (shakeCount > 5)
    {
        DamageMe = false;
        shakeCount = 0;
        return;
    }
    if (elapsed > timePerFrame)
    {
		BattleMenuQuad.x += shakeAmount;
		PartyHealthBarQuad.x += shakeAmount;
        shakeAmount = -shakeAmount;
        elapsed = 0.0;
        shakeCount++;
    }
}

void UI::ResetSliders()
{
    EnemyHealthBarSlider.elapsed = 0.0f;
    BattleMenuSlider.elapsed = 0.0f;
    PartyHealthBarSlider.elapsed = 0.0f;
}

void UI::OnBattlePhaseChange(BattlePhase bp)
{
    if (bp == BattlePhase::Start)
    {
        CurrentCharName = BattleSystem.PartyList[BattleSystem.PartyListIndex];
    }
    else if (bp == BattlePhase::End)
    {
        ResetSliders();

        BattleMenuQuad.x = BattleMenuOnScreenX - OffScreenDistance;

        EnemyHealthBarQuad.x = EnemyHealthBarOnScreenX + OffScreenDistance; //set it up for sliding on to screen
        EnemyHealthBarQuad.width = EnemyHealthStartWidth;

        //remember where health is for next battle
        PartyHealthBarSlider.end = PartyHealthBarQuad.x;
    }

    CurrentBP = bp;
}

void UI::OnTurnAction(TurnAction& ta)
{
    if (ta.User == Side::Party)
    {
        PlayerTurn = true;
        CurrentTurnText = ta.Name + '!';
    }
    else if (ta.User == Side::Enemy)
    {
        std::cout << "Damage: " << ta.DamagePoints << std::endl;
        if (ta.DamagePoints > 0)
            DamageMe = true;

        PlayerTurn = false;
        CurrentTurnText = ta.Name + '!';
    }

    EnemyHealthBarQuad.width = EnemyHealthStartWidth * (BattleSystem.EnemyCurrentHealthPercent / 100);
    EnemyHealthBarQuad.x = EnemyHealthBarOnScreenX - (EnemyHealthStartWidth - EnemyHealthBarQuad.width) / 2;

    PartyHealthBarQuad.width = PartyHealthStartWidth * (BattleSystem.PartyCurrentHealthPercent / 100);
    PartyHealthBarQuad.x = PartyHealthBarOnScreenX - (PartyHealthStartWidth - PartyHealthBarQuad.width) / 2;
}

void UI::OnCharacterTurnChange(std::string charname)
{
    CurrentCharName = charname;
}
