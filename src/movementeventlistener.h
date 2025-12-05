#ifndef MOVEMENTEVENTLISTENER_H
#define MOVEMENTEVENTLISTENER_H

#include "inputaction.h"
#include "cardinal.h"

class MovementEventListener
{
public:
    virtual void OnMoveActionChange(InputAction) = 0;
    virtual void OnDirectionChange(Cardinal) = 0;
    virtual void OnMoveDistanceChange(float) = 0;
    virtual ~MovementEventListener() = default;
};
#endif