#ifndef MOVEMENTEVENTLISTENER_H
#define MOVEMENTEVENTLISTENER_H

#include "moveaction.h"

class MovementEventListener
{
public:
    virtual void OnMoveActionChange(MoveAction ma) = 0;
    virtual ~MovementEventListener() = default;
};
#endif