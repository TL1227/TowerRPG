#include "ui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui.h"
#include "text.h"

static float TopMarg = 58;
static float MenuTextLeftX;
static float multi = 0.2f;

static float EnemyHealthStartWidth = 0.0f;
static float EnemyHealthStartX = 0.0f;

static float PartyHealthStartWidth = 0.0f;
static float PartyHealthStartX = 0.0f;

UI::UI(float preambleDuration, ::BattleSystem& battleSystem, int screenHeight, int screenWidth)
    : BattleSystem{ battleSystem }
    , ScreenHeight{ screenHeight }
    , ScreenWidth{ screenWidth }
{
    //This is so our text can scale with our screensize
    ScreenScale = (float)ScreenWidth / 960;
    TextScale = ScreenScale * 0.43f;

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(ScreenWidth), 0.0f, static_cast<float>(ScreenHeight));
    OffScreenDistance = (float)ScreenWidth * 2.0f;

    Shader textShader{ "shaders\\uivert.shader", "shaders\\uifrag.shader" };
    textShader.use();
	textShader.setMat4("projection", projection);
    BattleMenuText = { ScreenWidth, ScreenHeight, textShader };
    CurrentCharNameText = { ScreenWidth, ScreenHeight, textShader };

    //TODO: these should probably be in some kind of UI component class
    Shader battleMenuShader{ "shaders\\battleuivert.shader", "shaders\\battleuifrag.shader" };
    battleMenuShader.use();
	battleMenuShader.setMat4("projection", projection);

    BattleMenuQuad = { "textures\\battlemenu.jpg", battleMenuShader};
    BattleMenuQuad.width = (float)ScreenWidth * 0.65f;
    BattleMenuQuad.height = (float)ScreenHeight * 0.20f;
    BattleMenuQuad.x = -BattleMenuQuad.width;
    BattleMenuOnScreenY = BattleMenuQuad.height / 2.0f; //snap to bottom of screen
    BattleMenuQuad.y = BattleMenuOnScreenY;
    BattleMenuOnScreenX = (float)ScreenWidth / 2.0f; 
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
    EnemyHealthBarOnScreenX = ScreenWidth / 2.0f;
    EnemyHealthBarOnScreenY = (float)ScreenHeight * 0.93f;
    EnemyHealthBarQuad.y = EnemyHealthBarOnScreenY;
    EnemyHealthBarQuad.width = (float)ScreenWidth * 0.65f;
    EnemyHealthBarQuad.height = (float)ScreenHeight * 0.03f;
    EnemyHealthBarQuad.x = screenWidth + EnemyHealthBarQuad.width;

    EnemyHealthStartWidth = EnemyHealthBarQuad.width;
    EnemyHealthStartX = EnemyHealthBarQuad.x;

    EnemyHealthBarSlider.duration = preambleDuration;
	EnemyHealthBarSlider.start = EnemyHealthBarQuad.x;
    EnemyHealthBarSlider.end = EnemyHealthBarOnScreenX;

    //PartyHP
    PartyHealthBarQuad = { "textures\\partyhealthbar.jpg", enemyHpShader };
    PartyHealthBarOnScreenX = ScreenWidth / 2.0f;
    //PartyHealthBarOnScreenY = (float)ScreenHeight * 0.93f;
    PartyHealthBarQuad.width = (float)ScreenWidth * 0.65f;
    PartyHealthBarQuad.height = (float)ScreenHeight * 0.03f;
    PartyHealthBarQuad.y = BattleMenuQuad.Top() + (PartyHealthBarQuad.height / 2);
    PartyHealthBarQuad.x = -PartyHealthBarQuad.width;

    PartyHealthStartWidth = PartyHealthBarQuad.width;
    PartyHealthStartX = PartyHealthBarQuad.x;

    PartyHealthBarSlider.duration = preambleDuration;
	PartyHealthBarSlider.start = PartyHealthBarQuad.x;
    PartyHealthBarSlider.end = PartyHealthBarOnScreenX;
}

void UI::Tick(float deltaTime)
{
    /*
    ImGui::Begin("Battle Menu");
    ImGui::SliderFloat("TextChoicex", &MenuTextLeftX, 0, 1000);
    ImGui::SliderFloat("BattleMenux", &BattleMenuQuad.x, 0, 1000);
    ImGui::End();
    */
    CurrentBP = BattleSystem.GetPhase();

    if (CurrentBP == BattlePhase::Preamble)
    {
        BattleMenuText.Draw("Grrrrr... I'm a Goblin!", 0, 200, 1, glm::vec3(1.0, 0.5, 0.5), TextAlign::Center);
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

        //TODO: can this just be BattleMenuTexttoo?
        BattleMenuText.Draw(CurrentCharName, CharNameX, CharNameY, TextScale, HighlightColour);

        //TODO: stop calculating text pos on every frame
		for (size_t i = 0; i < BattleSystem.BattleMenuChoiceSize; i++)
		{
            glm::vec3 tColour = (i == BattleSystem.BattleMenuChoiceIndex) ? HighlightColour : NoHighlightColour;
            float textY = BattleMenuQuad.Top() - (TopMarg * TextScale * (i + 1));

            BattleMenuText.Draw(BattleSystem.BattleMenuText[i], MenuTextLeftX, textY, TextScale, tColour);
		}
    }
    else if (CurrentBP == BattlePhase::ExecuteTurn)
    {
        BattleMenuQuad.Draw();
        EnemyHealthBarQuad.Draw();
        PartyHealthBarQuad.Draw();

        //TODO: Why isn't this centering properly (>_<)
        BattleMenuText.Draw(CurrentTurnText, 0, BattleMenuOnScreenY, TextScale, HighlightColour, TextAlign::Center);
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
    PartyHealthBarSlider.elapsed = 0.0f;
}

void UI::OnBattlePhaseChange(BattlePhase bp)
{
    if (bp == BattlePhase::Start)
    {
        CurrentCharName = BattleSystem.CurrentPartyMember;
    }
    else if (bp == BattlePhase::End)
    {
        ResetSliders();

        BattleMenuQuad.x = BattleMenuOnScreenX - OffScreenDistance;

        EnemyHealthBarQuad.x = EnemyHealthBarOnScreenX + OffScreenDistance; //set it up for sliding on to screen
        EnemyHealthBarQuad.width = EnemyHealthStartWidth;
    }
}

void UI::OnMenuActionButtonPress(MenuAction button)
{

}

void UI::OnTurnAction(TurnAction& ta)
{
    float damageDone = EnemyHealthStartWidth * (ta.DamagePercent / 100);
    EnemyHealthBarQuad.width -= damageDone;
    EnemyHealthBarQuad.x -= (damageDone / 2);
    CurrentTurnText = ta.Name + '!';
}

void UI::OnCharacterTurnChange(std::string charname)
{
    CurrentCharName = charname;
}
