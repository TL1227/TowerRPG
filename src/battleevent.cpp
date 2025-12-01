#include "battleevent.h"

void BattleEvent::AddListener(BattleEventListener& ls)
{
    Listeners.push_back(&ls);
}

void BattleEvent::OnPhaseChange(BattlePhase bp)
{
    for (auto* lisenter : Listeners)
        lisenter->OnPhaseChange(bp);
}
