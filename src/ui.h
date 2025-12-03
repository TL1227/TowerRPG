#ifndef UI_H
#define UI_H

#include "battlesystem.h"
#include "battleeventlistener.h"
#include "quad.h"
#include "text.h"

struct Slider {
    float start;
    float end;
    float elapsed = 0.0f;
    float duration = 2.6f;   // seconds
};

class UI : public BattleEventListener
{
public:
	UI(float, BattleSystem&, int screenHeight, int screenWidth);
    void ResetSliders();
    void Tick(float delta);

private:
	int ScreenHeight;
	int ScreenWidth;
    float OffScreenDistance;

    //TODO: make a ui element struct?
    float BattleMenuOnScreenY;
	Slider BattleMenuSlider;

	Quad EnemyHealthBar;
    float EnemyHealthBarOnScreenY;
	Slider EnemyHealthBarSlider;

    bool IsSliding = false;
    bool Slide(float, float&, Slider&);

	Quad BattleMenu;
    BattleSystem &BattleSystem;
    bool IsBattle = false;

    Text Text;

    void OnBattlePhaseChange(BattlePhase) override;
};

#endif
