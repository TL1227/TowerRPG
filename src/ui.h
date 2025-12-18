#ifndef UI_H
#define UI_H

#include "battlesystem.h"
#include "battleeventlistener.h"
#include "inputeventlistener.h"
#include "quad.h"
#include "text.h"

struct Slider {
    float start;
    float end;
    float elapsed = 0.0f;
    float duration = 2.6f;   // seconds
};

class UI : public BattleEventListener, public InputEventListener
{
public:
	UI(float, BattleSystem&, int screenHeight, int screenWidth);
    void Tick(float delta);

private:
	int ScreenHeight;
	int ScreenWidth;
    float ScreenScale;
    float TextScale;
    float OffScreenDistance;

    //TODO: make a ui element struct?
	Quad BattleMenuQuad;
    float BattleMenuOnScreenY;
    float BattleMenuOnScreenX;
	Slider BattleMenuSlider;
    Text BattleMenuText;
    std::string CurrentCharName;
    Text CurrentCharNameText;
    float CharNameX;
    float CharNameY;
    std::string CurrentTurnText;
    glm::vec3 HighlightColour = glm::vec3{ 1.0, 1.0, 1.0 };
    glm::vec3 NoHighlightColour = glm::vec3{ 0.5, 0.5, 0.5 };

	Quad EnemyHealthBarQuad;
    float EnemyHealthBarOnScreenY;
    float EnemyHealthBarOnScreenX;
	Slider EnemyHealthBarSlider;

	Quad PartyHealthBarQuad;
    float PartyHealthBarOnScreenY;
    float PartyHealthBarOnScreenX;
	Slider PartyHealthBarSlider;

    void ResetSliders();
    bool Slide(float, float&, Slider&);
    BattleSystem &BattleSystem;

    BattlePhase CurrentBP;


    void OnBattlePhaseChange(BattlePhase) override;
    void OnTurnAction(TurnAction&) override;
    void OnMenuActionButtonPress(MenuAction button) override;
    void OnCharacterTurnChange(std::string charname) override;
};

#endif
