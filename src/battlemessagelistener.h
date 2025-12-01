#ifndef BATTLEMESSAGE_LISTENER_H
#define BATTLEMESSAGE_LISTENER_H

enum class BattlePhase
{
	Sighting,
	Preamble,
	Slide,
	Snap,
	Start,
	End
};

class BattleMessageListener
{
public:
    virtual void PhaseChange(BattlePhase bp);
};

#endif
