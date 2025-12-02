#ifndef UI_H
#define UI_H

#include "quad.h"

struct Slider {
    float start;
    float end;
    float elapsed = 0.0f;
    float duration = 2.6f;   // seconds
};

class UI
{
public:
	UI();

private:
	Quad BattleMenu{};
	Slider BattleMenuSlider;
	Quad EnemyHealthBar;
	Slider EnemyHealthBarSlider;
	int ScreenHeight;
	int ScreenWidth;
};

#endif