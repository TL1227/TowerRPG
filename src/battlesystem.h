#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include "battleevent.h"
#include "inputeventlistener.h"
#include "enemy.h"

enum class BattleMenuChoice
{
    Attack,
    Skill,
    Item,
    Run
};

class BattleSystem : public InputEventListener
{
public:
	void SetBattlePhase(BattlePhase);
	BattlePhase GetPhase();
	void AutoMoveFinished();
	void DecreaseEnemyCounter();
	void Tick(float delta);
	double PreambleStartTime;
	double PreambleLength = 1; //TODO: set this using battle intro length
	BattleEvent* BattleEvent;
    Enemy* Enemy;

    BattleMenuChoice BattleMenuCurrentChoice = BattleMenuChoice::Attack;
    int BattleMenuChoiceIndex = 0;
	std::vector<std::string> BattleMenuText = { "Attack", "Skill", "Item", "Run" };
    int BattleMenuChoiceSize = (int)BattleMenuText.size();

private:
	BattlePhase CurrentBattlePhase = BattlePhase::End;
	int EnemyCounter = 1;

    void OnMenuActionButtonPress(MenuAction button) override;
};

#endif

