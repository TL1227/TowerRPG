#ifndef BATTLEEVENT_LISTENER_H
#define BATTLEEVENT_LISTENER_H

#include "battlephase.h"

class BattleEventListener
{
public:
    virtual void OnPhaseChange(BattlePhase bp) = 0;
    virtual ~BattleEventListener() = default;
};

#endif
