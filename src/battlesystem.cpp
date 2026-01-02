#include "battlesystem.h"
#include "battlephase.h"
#include "menuaction.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui.h"

void BattleSystem::Tick(float delta)
{
    ImGui::Begin("BattleSystem");
    ImGui::Text("Phase: %s", GetBattlePhaseText(CurrentBattlePhase).c_str());
    ImGui::Text("BattleChoices Size: %zi", TurnActions.size());
    ImGui::Text("CurrentPartyListIndex : %i", PartyListIndex);
    ImGui::Text("SkillListIndex : %i", SkillListIndex);
    ImGui::Text("SkillListSize : %i", SkillListSize);
    ImGui::End();

	if (CurrentBattlePhase == BattlePhase::Preamble)
	{
		double timepassed = glfwGetTime() - PreambleStartTime;

		if (timepassed >= PreambleLength)
		{
			SetBattlePhase(BattlePhase::Slide);
		}
	}
	else if (CurrentBattlePhase == BattlePhase::ExecuteTurn)
    {
		if (TurnActions[CurrentChoiceIndex].IsFinished())
		{
            CurrentChoiceIndex++;

			if (CurrentChoiceIndex >= TurnActions.size())
			{
				std::cout << "Setting StartTurn" << std::endl;
				SetBattlePhase(BattlePhase::StartTurn);
			}
			else
			{
				ExecuteTurnAction(TurnActions[CurrentChoiceIndex]);
			}
		}
		else
		{
			TurnActions[CurrentChoiceIndex].Elapsed += delta;
		}
    }
}

std::string BattleSystem::GetBattlePhaseText(BattlePhase phase)
{
    switch(phase)
    {
        case BattlePhase::Sighting: return "Sighting";
        case BattlePhase::Preamble: return "Preamble";
        case BattlePhase::Slide: return "Slide";
        case BattlePhase::Snap: return "Snap";
        case BattlePhase::Start: return "Start";
        case BattlePhase::ChoosingSkill: return "ChoosingSkill";
        case BattlePhase::StartTurn: return "StartTurn";
        case BattlePhase::ExecuteTurn: return "ExecuteTurn";
        case BattlePhase::End: return "End";
    }
}

std::string BattleSystem::GetBattleMenuText(BattleMenuChoice choice)
{
    switch(choice)
    {
        case BattleMenuChoice::Attack: return "Attack";
        case BattleMenuChoice::Skill: return "Skill";
        case BattleMenuChoice::Item: return "Item";
        case BattleMenuChoice::Run: return "Run";
        case BattleMenuChoice::LENGTH: return "LENGTH";
    }
}

void BattleSystem::SetBattlePhase(BattlePhase phase)
{
	CurrentBattlePhase = phase;

    std::cout << GetBattlePhaseText(CurrentBattlePhase) << std::endl;

    if (CurrentBattlePhase == BattlePhase::Preamble)
    {
        PreambleStartTime = glfwGetTime();
    }
    else if (CurrentBattlePhase == BattlePhase::StartTurn)
    {
        CurrentChoiceIndex = 0;
        TurnActions.clear();
    }
    else if (CurrentBattlePhase == BattlePhase::End)
    {
		PartyListIndex = 0;

        CurrentChoiceIndex = 0;
        BattleMenuIndex = 0;
        TurnActions.clear();

		ChangePartyMember(PartyList[PartyListIndex]);
    }

	BattleEvent->DispatchPhaseChange(phase);

    return;
}

BattlePhase BattleSystem::GetPhase()
{
    return CurrentBattlePhase;
}

void BattleSystem::AutoMoveFinished()
{
	if (CurrentBattlePhase == BattlePhase::Sighting)
	{
		SetBattlePhase(BattlePhase::Preamble);
	}
}

void BattleSystem::ExecuteTurnAction(TurnAction choice)
{
    std::cout << choice.Name << std::endl;

    BattleEvent->DispatchTurnAction(choice);

    if (Enemy->HealthPoints <= 0)
    {
        SetBattlePhase(BattlePhase::End);
    }
}

void BattleSystem::DecreaseEnemyCounter()
{
	EnemyCounter--;

	if (EnemyCounter <= 0)
	{
		SetBattlePhase(BattlePhase::Sighting);
		EnemyCounter = 5;
	}
}

void BattleSystem::ChangePartyMember(std::string member)
{
    BattleEvent->DispatchCharacterTurnChange(member);
}

void BattleSystem::AddEnemyChoice()
{
	float damagePoints = 20;
    //TODO: maybe get this from party max health
	float onepercent = Enemy->MaxHealth / 100;
	float percentDamage = onepercent * damagePoints;

	TurnAction ta;
	ta.Name = "Goblin Slash!";
	ta.DamagePercent = percentDamage;
	ta.DamagePoints = damagePoints;
    ta.TargetsEnemy = false;
    ta.ActionTime = 1.5;

	TurnActions.push_back(ta);
}

void BattleSystem::SetChoiceOrder()
{
    //figure out speed and such
}

void BattleSystem::MenuUp(int &index, int size)
{
    if (--index < 0) { index = size - 1; }
}

void BattleSystem::MenuDown(int &index, int size)
{
    if (++index >= size) { index = 0; }
}

void BattleSystem::OnMenuActionButtonPress(MenuAction ma)
{
    if (ma == MenuAction::Up)
    {
        if (CurrentBattlePhase == BattlePhase::ChoosingSkill) 
            MenuUp(SkillListIndex, SkillListSize);
        else 
            MenuUp(BattleMenuIndex, BattleMenuSize);
    }
    else if (ma == MenuAction::Down)
    {
        if (CurrentBattlePhase == BattlePhase::ChoosingSkill)
            MenuDown(SkillListIndex, SkillListSize);
        else
            MenuDown(BattleMenuIndex, BattleMenuSize);
    }
    else if (ma == MenuAction::Confirm)
    {
        if (CurrentBattlePhase == BattlePhase::ChoosingSkill)
        {
            float damagePoints = 18;
            float onepercent = Enemy->MaxHealth / 100;
            float percentDamage = onepercent * damagePoints;

            TurnAction ta;
            ta.Name = SkillList[SkillListIndex];
            ta.DamagePercent = percentDamage;
            ta.DamagePoints = damagePoints;
            ta.TargetsEnemy = true;

            TurnActions.push_back(ta);

            SkillListIndex = 0;

            SetBattlePhase(BattlePhase::Start);
        }
        else if (CurrentBattlePhase == BattlePhase::Start || CurrentBattlePhase == BattlePhase::StartTurn)
        {
            if ((BattleMenuChoice)BattleMenuIndex == BattleMenuChoice::Attack) 
            {
                float damagePoints = 5;
                float onepercent = Enemy->MaxHealth / 100;
                float percentDamage = onepercent * damagePoints;

                //TODO: maybe use constructor?
                TurnAction ta;
                ta.Name = "Attack";
                ta.DamagePercent = percentDamage;
                ta.DamagePoints = damagePoints;
                ta.TargetsEnemy = true;

                TurnActions.push_back(ta);
            }
            else if ((BattleMenuChoice)BattleMenuIndex == BattleMenuChoice::Skill) 
            {
                //open skill menu
                SetBattlePhase(BattlePhase::ChoosingSkill);

                SkillList.clear();

                auto CurrentPartyMember = PartyList[PartyListIndex];

                if (CurrentPartyMember == "Warrior")
                {
                    SkillList.push_back("Cleave");
                    SkillList.push_back("Heavy Cleave");
                }
                if (CurrentPartyMember == "Witch")
                {
                    SkillList.push_back("Fire");
                    SkillList.push_back("Lightning");
                }
                if (CurrentPartyMember == "Cleric")
                {
                    SkillList.push_back("Holy");
                    SkillList.push_back("Pray");
                }
                if (CurrentPartyMember == "Thief")
                {
                    SkillList.push_back("Stab");
                    SkillList.push_back("Steal");
                }

                SkillListSize = SkillList.size();

                return;
            }
            else if ((BattleMenuChoice)BattleMenuIndex == BattleMenuChoice::Item) 
            {
                TurnAction ta;
                ta.Name = "Item";
                ta.DamagePercent = 0;
                ta.DamagePoints = 0;
                ta.TargetsEnemy = true;

                TurnActions.push_back(ta);
            }
            else if ((BattleMenuChoice)BattleMenuIndex == BattleMenuChoice::Run) 
            {
                SetBattlePhase(BattlePhase::End);
                return;
            }
        }

        if (TurnActions.size() >= 4)
        {
            AddEnemyChoice();
            SetChoiceOrder();
            SetBattlePhase(BattlePhase::ExecuteTurn);
            ExecuteTurnAction(TurnActions[CurrentChoiceIndex]);

            PartyListIndex = 0;
            ChangePartyMember(PartyList[PartyListIndex]);
        }
        else
        {
            ChangePartyMember(PartyList[++PartyListIndex]);
        }
    }
}
