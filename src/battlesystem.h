#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <vector>
#include <string>

#include "inputeventlistener.h"
#include "skillparse.h"
#include "battleevent.h"

class TurnAction;
class Enemy;
enum class BattlePhase;

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
    BattleSystem();
	void SetBattlePhase(BattlePhase);
	BattlePhase GetPhase() const;
	void AutoMoveFinished();
	void DecreaseEnemyCounter();
	void Tick(float delta);
    void SubscribeToEvents(BattleEventListener&);
	double PreambleStartTime;
	double PreambleLength = 2.4; //TODO: set this using battle intro length
    
    int PartyMaxHealth = 100;
    int PartyCurrentHealth = 100;
    float PartyCurrentHealthPercent = 100;

    int EnemyMaxHealth;
    int EnemyCurrentHealth;
    float EnemyCurrentHealthPercent = 100;

    BattleEvent Events{};
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

    void SubmitAttack();

private:
	BattlePhase CurrentBattlePhase;
	int EnemyCounter = 5;
    void ExecuteTurnAction(TurnAction choice);
    void ChangePartyMember(std::string member);
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

