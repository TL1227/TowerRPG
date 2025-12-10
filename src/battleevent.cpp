#include "battleevent.h"

void BattleEvent::AddListener(BattleEventListener& ls)
{
    Listeners.push_back(&ls);
}

void BattleEvent::DispatchPhaseChange(BattlePhase bp)
{
	for (auto* lisenter : Listeners)
		lisenter->OnBattlePhaseChange(bp);
}

void BattleEvent::DispatchEnemyDamage(float dmgPercent)
{
	for (auto* lisenter : Listeners)
		lisenter->OnEnemyDamage(dmgPercent);
}
