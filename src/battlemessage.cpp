#include "battlemessage.h"

#include <GLFW/glfw3.h>
#include <iostream>


void BattleMessageAnnouncer::SetBattlePhase(BattlePhase phase)
{
    for(auto& lisenter : Listeners)
    {
        lisenter.PhaseChange(phase);
    }

    return;

	CurrentPhase = phase;

	if (CurrentPhase == BattlePhase::Preamble)
	{
		PreambleStartTime = glfwGetTime();
		Audio->PlayPreBattleBgm();
	}
	else if (CurrentPhase == BattlePhase::Snap)
	{
		Audio->PlayBattleBgm();
		Audio->StopPreBattleBgm();
	}
	else if (CurrentPhase == BattlePhase::End)
	{
		Audio->StopBattleBgm();
	}

	std::cout << "CurrentPhase: " << (int)phase << " Time: " << glfwGetTime() << std::endl;
}

void BattleMessageAnnouncer::DecreaseEnemyCounter()
{
	EnemyCounter--;
	std::cout << EnemyCounter << std::endl;

	if (EnemyCounter <= 0)
	{
		SetBattlePhase(BattlePhase::Sighting);
		EnemyCounter = 5;
	}
}
