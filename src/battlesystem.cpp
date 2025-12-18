#include "battlesystem.h"
#include "battlephase.h"
#include "menuaction.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void BattleSystem::Tick(float delta)
{
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
		if (BattleChoices[CurrentChoiceIndex].IsFinished())
		{
            CurrentChoiceIndex++;

			if (CurrentChoiceIndex >= BattleChoices.size())
			{
				std::cout << "Setting StartTurn" << std::endl;
				SetBattlePhase(BattlePhase::StartTurn);
			}
			else
			{
				ExecuteTurnAction(BattleChoices[CurrentChoiceIndex]);
			}
		}
		else
		{
			BattleChoices[CurrentChoiceIndex].Elapsed += delta;
		}
    }
}

void BattleSystem::PrintBattlePhase(BattlePhase phase)
{
    switch(phase)
    {
        case BattlePhase::Sighting:
            std::cout << "Sighting" << std::endl;
            break;
        case BattlePhase::Preamble:
            std::cout << "Preamble" << std::endl;
            break;
        case BattlePhase::Slide:
            std::cout << "Slide" << std::endl;
            break;
        case BattlePhase::Snap:
            std::cout << "Snap" << std::endl;
            break;
        case BattlePhase::Start:
            std::cout << "Start" << std::endl;
            break;
        case BattlePhase::StartTurn:
            std::cout << "StartTurn" << std::endl;
            break;
        case BattlePhase::ExecuteTurn:
            std::cout << "ExecuteTurn" << std::endl;
            break;
        case BattlePhase::End:
            std::cout << "End" << std::endl;
            break;
        }
}

void BattleSystem::SetBattlePhase(BattlePhase phase)
{
	CurrentBattlePhase = phase;

    PrintBattlePhase(CurrentBattlePhase);

    if (CurrentBattlePhase == BattlePhase::Preamble)
    {
        PreambleStartTime = glfwGetTime();
    }
    else if (CurrentBattlePhase == BattlePhase::StartTurn)
    {
        CurrentChoiceIndex = 0;
        BattleChoices.clear();
    }
    else if (CurrentBattlePhase == BattlePhase::End)
    {
        CurrentChoiceIndex = 0;
        BattleChoices.clear();
		CurrentPartyListIndex = 0;
		ChangePartyMember(PartyList[CurrentPartyListIndex]);
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
    CurrentPartyMember = member;
    BattleEvent->DispatchCharacterTurnChange(member);
}

void BattleSystem::AddEnemyChoice()
{
	float damagePoints = 20;
    //TODO: maybe get this from party max health
	float onepercent = Enemy->MaxHealth / 100;
	float percentDamage = onepercent * damagePoints;

	TurnAction ta;
	ta.Name = "Goblin Deez Nuts";
	ta.DamagePercent = percentDamage;
	ta.DamagePoints = damagePoints;
    ta.TargetsEnemy = false;
    ta.ActionTime = 1.5;

	BattleChoices.push_back(ta);
}

void BattleSystem::SetChoiceOrder()
{
    //figure out speed and such
}

void BattleSystem::OnMenuActionButtonPress(MenuAction ma)
{
    if (ma == MenuAction::Up)
    {
        if (--BattleMenuChoiceIndex < 0)
        {
            BattleMenuChoiceIndex = BattleMenuText.size() - 1;
        }

        BattleMenuCurrentChoice = (BattleMenuChoice)BattleMenuChoiceIndex;
    }
    else if (ma == MenuAction::Down)
    {
        if (++BattleMenuChoiceIndex >= BattleMenuText.size())
        {
            BattleMenuChoiceIndex = 0;
        }

        BattleMenuCurrentChoice = (BattleMenuChoice)BattleMenuChoiceIndex;
    }
    else if (ma == MenuAction::Confirm)
    {
        std::cout << "confirm" << std::endl;
        if (BattleMenuCurrentChoice == BattleMenuChoice::Attack) 
        {
            float damagePoints = 5;
            float onepercent = Enemy->MaxHealth / 100;
            float percentDamage = onepercent * damagePoints;

            //TODO: maybe use ctor
            TurnAction ta;
            ta.Name = "Attack";
            ta.DamagePercent = percentDamage;
            ta.DamagePoints = damagePoints;
            ta.TargetsEnemy = true;

            BattleChoices.push_back(ta);

        }
        else if (BattleMenuCurrentChoice == BattleMenuChoice::Skill) 
        {
            float damagePoints = 18;
            float onepercent = Enemy->MaxHealth / 100;
            float percentDamage = onepercent * damagePoints;

            TurnAction ta;
            ta.Name = "Skill";
            ta.DamagePercent = percentDamage;
            ta.DamagePoints = damagePoints;
            ta.TargetsEnemy = true;

            BattleChoices.push_back(ta);
        }
        else if (BattleMenuCurrentChoice == BattleMenuChoice::Item) 
        {
        }
        else if (BattleMenuCurrentChoice == BattleMenuChoice::Run) 
        {
            SetBattlePhase(BattlePhase::End);
        }

        if (BattleChoices.size() >= 4)
        {
            AddEnemyChoice();
            SetChoiceOrder();
            SetBattlePhase(BattlePhase::ExecuteTurn);
            ExecuteTurnAction(BattleChoices[CurrentChoiceIndex]);

            CurrentPartyListIndex = 0;
            ChangePartyMember(PartyList[CurrentPartyListIndex]);
        }
        else
    {
            ChangePartyMember(PartyList[++CurrentPartyListIndex]);
        }
    }
}
