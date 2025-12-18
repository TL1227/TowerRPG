#ifndef BATTLEEVENT_LISTENER_H
#define BATTLEEVENT_LISTENER_H

#include <string>

#include "battlephase.h"
#include "turnaction.h"

class BattleEventListener
{
public:
    virtual void OnBattlePhaseChange(BattlePhase bp) {};
    virtual void OnTurnAction(TurnAction& turnAction){};
    virtual void OnCharacterTurnChange(std::string charname){};
    virtual ~BattleEventListener() = default;
};

#endif
