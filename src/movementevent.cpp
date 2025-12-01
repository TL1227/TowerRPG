#include "movementevent.h"

void MovementEvent::AddListener(MovementEventListener& ls)
{
    Listeners.push_back(&ls);
}

void MovementEvent::OnMoveActionChange(MoveAction ma)
{
    for (auto* lisenter : Listeners)
        lisenter->OnMoveActionChange(ma);
}