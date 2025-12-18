#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include "battleevent.h"
#include "inputeventlistener.h"
#include "enemy.h"
#include "turnaction.h"

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
    std::vector<BattleMenuChoice> BattleChoices;
    int CurrentChoiceIndex = 0;
    bool ExecutingChoice = false;
    int TurnStartTime = 0;

    TurnAction CurrentTurnAction;

    int BattleMenuChoiceIndex = 0;
    BattleMenuChoice BattleMenuCurrentChoice = BattleMenuChoice::Attack;
	std::vector<std::string> BattleMenuText = { "Attack", "Skill", "Item", "Run" };
    int BattleMenuChoiceSize = (int)BattleMenuText.size();

    int CurrentPartyListIndex = 0;
	std::vector<std::string> PartyList = { "Warrior", "Wizard", "Cleric", "Theif" };
    std::string CurrentPartyMember = PartyList[CurrentPartyListIndex];
    int PartyMemberChoiceSize = (int)PartyList.size();

private:
	BattlePhase CurrentBattlePhase = BattlePhase::End;
	int EnemyCounter = 1;
    void ExecuteChoice(BattleMenuChoice choice);
    void ChangePartyMember(std::string member);

    void OnMenuActionButtonPress(MenuAction button) override;
};

#endif

