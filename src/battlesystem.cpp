#include "battlesystem.h"

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
