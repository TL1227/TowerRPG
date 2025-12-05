#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <vector>

#include "battlephase.h"
#include "battleevent.h"
#include "battleeventlistener.h"
#include "audio.h"

class BattleSystem
{
public:
	void SetBattlePhase(BattlePhase);
	BattlePhase GetPhase();
	void AutoMoveFinished();
	void DecreaseEnemyCounter();
	void Tick(float delta);
	Audio* Audio;
	double PreambleStartTime;
	double PreambleLength = 1; //TODO: set this using battle intro length
	BattleEvent* BattleEvent;
private:
	BattlePhase CurrentBattlePhase = BattlePhase::End;
	int EnemyCounter = 5;
};

#endif

