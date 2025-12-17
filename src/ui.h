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
    Text Text;

    void OnBattlePhaseChange(BattlePhase) override;
    void OnEnemyDamage(float dmgPercent) override;
    void OnMenuActionButtonPress(MenuAction button) override;
};

#endif
