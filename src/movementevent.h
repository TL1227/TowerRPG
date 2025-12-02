#ifndef MOVEMENTEVENT_H
#define MOVEMENTEVENT_H

#include "movementeventlistener.h"
#include <vector>

class MovementEvent
{
public:
	void AddListener(MovementEventListener&);
	void DispatchMoveActionChange(MoveAction);
	void DispatchDirectionChange(Cardinal);
	void DispatchMoveDistanceChange(float);

private:
	std::vector<MovementEventListener*> Listeners;
};

#endif
