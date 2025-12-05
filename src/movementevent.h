#ifndef MOVEMENTEVENT_H
#define MOVEMENTEVENT_H

#include "movementeventlistener.h"
#include <vector>

class MovementEvent
{
public:
	void AddListener(MovementEventListener&);
	void DispatchMoveActionChange(InputAction);
	void DispatchDirectionChange(Cardinal);
	void DispatchMoveDistanceChange(float);

private:
	std::vector<MovementEventListener*> Listeners;
};

#endif
