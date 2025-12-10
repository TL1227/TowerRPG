#ifndef BATTLEEVENT_LISTENER_H
#define BATTLEEVENT_LISTENER_H

#include "battlephase.h"

class BattleEventListener
{
public:
    virtual void OnBattlePhaseChange(BattlePhase bp) {};
    virtual void OnEnemyDamage(float dmgPercent){};
    virtual ~BattleEventListener() = default;
};

#endif
