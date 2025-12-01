#ifndef BATTLEMESSAGE_H
#define BATTLEMESSAGE_H

#include "audio.h"
#include "battlemessagelistener.h"

#include <vector>


class BattleMessageAnnouncer
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
    std::vector<BattleMessageListener> Listeners;
};

#endif
