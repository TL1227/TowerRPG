#ifndef BATTLEEVENT_H
#define BATTLEEVENT_H

#include "battleeventlistener.h"
#include <vector>

class BattleEvent
{
public:
	void AddListener(BattleEventListener&);
	void DispatchPhaseChange(BattlePhase);

private:
	std::vector<BattleEventListener*> Listeners;
};

#endif