#ifndef BATTLEMESSAGE_H
#define BATTLEMESSAGE_H

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
	double PreambleLength = 2; //TODO: set this using battle intro length
	void SetBattlePhase(BattlePhase phase);
	void DecreaseEnemyCounter();
	bool UiSlideDone = false;
	bool UiSnapDone = false;
};

#endif