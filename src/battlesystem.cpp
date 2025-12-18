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
        if (CurrentTurnAction.IsFinished())
        {
            if (CurrentChoiceIndex >= BattleChoices.size())
            {
                SetBattlePhase(BattlePhase::StartTurn);
            }
            else
            {
                ExecuteChoice(BattleChoices[CurrentChoiceIndex++]);
            }
        }
        else
        {
            CurrentTurnAction.Elapsed += delta;
        }
    }
}

void BattleSystem::SetBattlePhase(BattlePhase phase)
{
	CurrentBattlePhase = phase;

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

void BattleSystem::ExecuteChoice(BattleMenuChoice choice)
{
    if (choice == BattleMenuChoice::Attack) 
    {
        float damagePoints = 5;
        float onepercent = Enemy->MaxHealth / 100;
        float percentDamage = onepercent * damagePoints;

        //TODO: maybe use ctor
        TurnAction ta;
        ta.DamagePercent = percentDamage;
        ta.DamagePoints = damagePoints;
        ta.TargetsEnemy = true;
        ta.ActionTime = 0.5;

        CurrentTurnAction = ta;

        BattleEvent->DispatchTurnAction(CurrentTurnAction);
    }
    else if (choice == BattleMenuChoice::Skill) 
    {
        float damagePoints = 18;
        float onepercent = Enemy->MaxHealth / 100;
        float percentDamage = onepercent * damagePoints;

        TurnAction ta;
        ta.DamagePercent = percentDamage;
        ta.DamagePoints = damagePoints;
        ta.TargetsEnemy = true;
        ta.ActionTime = 0.5;

        CurrentTurnAction = ta;

        BattleEvent->DispatchTurnAction(CurrentTurnAction);
    }
    else if (choice == BattleMenuChoice::Item) 
    {

    }
    else if (choice == BattleMenuChoice::Run) 
    {
        SetBattlePhase(BattlePhase::End);
    }

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
        BattleChoices.push_back(BattleMenuCurrentChoice);

        //TODO: switch to next party member()

        if (BattleChoices.size() >= 4)
        {
            SetBattlePhase(BattlePhase::ExecuteTurn);
            ExecuteChoice(BattleChoices[CurrentChoiceIndex++]);
        }
    }
}
