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
    ImGui::Text("EnemyHP : %i", EnemyCurrentHealth);
    ImGui::Text("EnemyHP%% : %f", EnemyCurrentHealthPercent);
    ImGui::Text("PartyHP : %i", PartyCurrentHealth);
    ImGui::Text("PartyHP%% : %f", PartyCurrentHealthPercent);
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

void BattleSystem::ExecuteTurnAction(TurnAction ta)
{
    std::cout << ta.Name << std::endl;

    if (ta.Target == Side::Enemy)
    {
        EnemyCurrentHealth = EnemyCurrentHealth -= ta.DamagePoints;
        EnemyCurrentHealthPercent = ((float)EnemyCurrentHealth / (float)EnemyMaxHealth) * 100.0f;
    }
    else if (ta.Target == Side::Party)
    {
        PartyCurrentHealth = PartyCurrentHealth -= ta.DamagePoints;
        PartyCurrentHealthPercent = ((float)PartyCurrentHealth / (float)PartyMaxHealth) * 100.0f;
    }

    BattleEvent->DispatchTurnAction(ta);

    if (EnemyCurrentHealth <= 0)
    {
        SetBattlePhase(BattlePhase::End);
    }
    if (EnemyCurrentHealth <= 0)
    {
        //TODO: player returns to start of map?
        SetBattlePhase(BattlePhase::End);
    }
}

void BattleSystem::SetBattlePhase(BattlePhase phase)
{
	CurrentBattlePhase = phase;

    std::cout << GetBattlePhaseText(CurrentBattlePhase) << std::endl;

    if (CurrentBattlePhase == BattlePhase::Preamble)
    {
        PreambleStartTime = glfwGetTime();

        EnemyMaxHealth = Enemy->MaxHealth;
        EnemyCurrentHealth = Enemy->MaxHealth;
    }
    if (CurrentBattlePhase == BattlePhase::Start)
    {
        EnemyCurrentHealthPercent = 100;
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
    }
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
	TurnAction ta = {"Goblin Slash", 30, Side::Party };
    ta.User = Side::Enemy;
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
            TurnAction ta = SkillList[SkillListIndex];
            ta.User = Side::Party;
            TurnActions.push_back(ta);

            SkillListIndex = 0;

            SetBattlePhase(BattlePhase::Start);
        }
        else if (CurrentBattlePhase == BattlePhase::Start || CurrentBattlePhase == BattlePhase::StartTurn)
        {
            if ((BattleMenuChoice)BattleMenuIndex == BattleMenuChoice::Attack) 
            {
                TurnAction ta = {"Attack", 5, Side::Enemy};
                ta.User = Side::Party;

                TurnActions.push_back(ta);
            }
            else if ((BattleMenuChoice)BattleMenuIndex == BattleMenuChoice::Skill) 
            {
                SetBattlePhase(BattlePhase::ChoosingSkill);

                SkillList.clear();

                auto CurrentPartyMember = PartyList[PartyListIndex];

                if (CurrentPartyMember == "Warrior")
                {
                    SkillList.push_back({"Cleave", 10, Side::Enemy });
                    SkillList.push_back({"Heavy Cleave", 18, Side::Enemy });
                }
                if (CurrentPartyMember == "Witch")
                {
                    SkillList.push_back({"Fire", 10, Side::Enemy });
                    SkillList.push_back({"Lightning", 18, Side::Enemy });
                }
                if (CurrentPartyMember == "Cleric")
                {
                    SkillList.push_back({"Pray", 10, Side::Enemy });
                    SkillList.push_back({"Heal", -20, Side::Party });
                }
                if (CurrentPartyMember == "Thief")
                {
                    SkillList.push_back({"Stab", 10, Side::Enemy });
                    SkillList.push_back({"Steal", 0, Side::Enemy });
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
                ta.Target = Side::Enemy;

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
