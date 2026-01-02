#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include "battleevent.h"
#include "battlephase.h"
#include "inputeventlistener.h"
#include "enemy.h"
#include "turnaction.h"

//TODO: this absolutely doesn't belong here!

enum class BattleMenuChoice
{
    Attack,
    Skill,
    Item,
    Run,
    LENGTH
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

    int CurrentChoiceIndex = 0;
    bool ExecutingChoice = false;
    int TurnStartTime = 0;

    std::vector<TurnAction> TurnActions;

    int BattleMenuIndex = 0;
	std::vector<std::string> BattleMenuList = { "Attack", "Skill", "Item", "Run" };
    int BattleMenuSize = (int)BattleMenuList.size();

    int PartyListIndex = 0;
	std::vector<std::string> PartyList = { "Warrior", "Witch", "Cleric", "Thief" }; //TODO: make this an enum
    int PartyListSize = (int)PartyList.size();

    int SkillListIndex = 0;
    std::vector<std::string> SkillList;
    int SkillListSize = 0;

private:
	BattlePhase CurrentBattlePhase = BattlePhase::End;
	int EnemyCounter = 1;
    void ExecuteTurnAction(TurnAction choice);
    void ChangePartyMember(std::string member);
    void SetChoiceOrder();
    void AddEnemyChoice();

    void MenuUp(int &index, int size);
    void MenuDown(int &index, int size);

    std::string GetBattlePhaseText(BattlePhase);
    std::string GetBattleMenuText(BattleMenuChoice);

    void OnMenuActionButtonPress(MenuAction button) override;
};

#endif

