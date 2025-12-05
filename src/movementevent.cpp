#include "movementevent.h"

void MovementEvent::AddListener(MovementEventListener& ls)
{
    Listeners.push_back(&ls);
}

void MovementEvent::DispatchMoveActionChange(InputAction ma)
{
    if (Listeners.size() > 0)
		for (auto* lisenter : Listeners)
			lisenter->OnMoveActionChange(ma);
}

void MovementEvent::DispatchDirectionChange(Cardinal c)
{
    if (Listeners.size() > 0)
		for (auto* lisenter : Listeners)
			lisenter->OnDirectionChange(c);
}

void MovementEvent::DispatchMoveDistanceChange(float f)
{
    if (Listeners.size() > 0)
		for (auto* lisenter : Listeners)
			lisenter->OnMoveDistanceChange(f);
}