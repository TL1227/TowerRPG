#ifndef MOVEMENTEVENTLISTENER_H
#define MOVEMENTEVENTLISTENER_H

#include "moveaction.h"
#include "cardinal.h"

class MovementEventListener
{
public:
    virtual void OnMoveActionChange(MoveAction) = 0;
    virtual void OnDirectionChange(Cardinal) = 0;
    virtual void OnMoveDistanceChange(float) = 0;
    virtual ~MovementEventListener() = default;
};
#endif