#ifndef BATTLEEVENT_LISTENER_H
#define BATTLEEVENT_LISTENER_H

#include "battlephase.h"
#include "turnaction.h"

class BattleEventListener
{
public:
    virtual void OnBattlePhaseChange(BattlePhase bp) {};
    virtual void OnTurnAction(TurnAction& turnAction){};
    virtual ~BattleEventListener() = default;
};

#endif
