#ifndef BATTLEMESSAGE_H
#define BATTLEMESSAGE_H

enum class BattlePhase
{
	Sighting,
	Preamble,
	Snapin,
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
};

#endif