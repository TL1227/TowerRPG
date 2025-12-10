#include "battlesystem.h"
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
}

void BattleSystem::SetBattlePhase(BattlePhase phase)
{
	CurrentBattlePhase = phase;

	if (CurrentBattlePhase == BattlePhase::Preamble)
	{
		PreambleStartTime = glfwGetTime();
	}

    std::cout << (int)CurrentBattlePhase << std::endl;

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
        std::cout << BattleMenuText[BattleMenuChoiceIndex] << std::endl;
    }
    else if (ma == MenuAction::Down)
    {
        if (++BattleMenuChoiceIndex >= BattleMenuText.size())
        {
			BattleMenuChoiceIndex = 0;
        }

        BattleMenuCurrentChoice = (BattleMenuChoice)BattleMenuChoiceIndex;
        std::cout << BattleMenuText[BattleMenuChoiceIndex] << std::endl;
    }
    else if (ma == MenuAction::Confirm)
    {
        if (BattleMenuCurrentChoice == BattleMenuChoice::Attack) 
        {
            float damagePoints = 5;

            float onepercent = Enemy->MaxHealth / 100;
            float percentDamage = onepercent * damagePoints;

            Enemy->HealthPoints -= damagePoints;
            BattleEvent->DispatchEnemyDamage(percentDamage);

            if (Enemy->HealthPoints <= 0)
            {
                SetBattlePhase(BattlePhase::End);
            }
        }
        else if (BattleMenuCurrentChoice == BattleMenuChoice::Skill) 
        {
            float damagePoints = 18;

            float onepercent = Enemy->MaxHealth / 100;
            float percentDamage = onepercent * damagePoints;

            Enemy->HealthPoints -= damagePoints;
            BattleEvent->DispatchEnemyDamage(percentDamage);

            if (Enemy->HealthPoints <= 0)
            {
                SetBattlePhase(BattlePhase::End);
            }
        }
        else if (BattleMenuCurrentChoice == BattleMenuChoice::Item) 
        {

        }
        else if (BattleMenuCurrentChoice == BattleMenuChoice::Run) 
        {
            SetBattlePhase(BattlePhase::End);
        }
    }
}
