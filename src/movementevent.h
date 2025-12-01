#ifndef MOVEMENTEVENT_H
#define MOVEMENTEVENT_H

#include "movementeventlistener.h"
#include <vector>

class MovementEvent
{
public:
	void AddListener(MovementEventListener&);
	void OnMoveActionChange(MoveAction);

private:
	std::vector<MovementEventListener*> Listeners;
};

#endif
