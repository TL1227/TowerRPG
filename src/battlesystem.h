#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include "battleevent.h"
#include "battlephase.h"
#include "inputeventlistener.h"
#include "enemy.h"
#include "turnaction.h"
#include "skillparse.h"

//TODO: this absolutely doesn't belong here!

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
	BattlePhase GetPhase() const;
	void AutoMoveFinished();
	void DecreaseEnemyCounter();
	void Tick(float delta);
	double PreambleStartTime;
	double PreambleLength = 2.4; //TODO: set this using battle intro length
    
    int PartyMaxHealth = 100;
    int PartyCurrentHealth = 100;
    float PartyCurrentHealthPercent = 100;

    int EnemyMaxHealth;
    int EnemyCurrentHealth;
    float EnemyCurrentHealthPercent = 100;

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
    std::vector<TurnAction> SkillList;
    int SkillListSize = 0;

private:
	BattlePhase CurrentBattlePhase = BattlePhase::End;
	int EnemyCounter = 1;
    void ExecuteTurnAction(TurnAction choice);
    void ChangePartyMember(std::string member) const;
    void SetChoiceOrder();
    void AddEnemyChoice();

    SkillParse SkillParser;

    void MenuUp(int& index, int size) const;
    void MenuDown(int &index, int size) const;

    std::string GetBattlePhaseText(BattlePhase);
    std::string GetBattleMenuText(BattleMenuChoice);

    void OnMenuActionButtonPress(MenuAction button) override;
};

#endif

