#ifndef BATTLEMESSAGE_H
#define BATTLEMESSAGE_H

#include "audio.h"

enum class BattlePhase
{
	Sighting,
	Preamble,
	Slide,
	Snap,
	Start,
	End
};

class BattleMessage
{
public:
	BattlePhase CurrentPhase = BattlePhase::End;
	int EnemyCounter = 3;
	double PreambleStartTime;
	double PreambleLength = 1; //TODO: set this using battle intro length
	void SetBattlePhase(BattlePhase phase);
	void DecreaseEnemyCounter();
	bool UiSlideDone = false;
	bool UiSnapDone = false;
	Audio* Audio;
};

#endif