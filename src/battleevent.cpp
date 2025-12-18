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

void BattleEvent::DispatchTurnAction(TurnAction& ta)
{
	for (auto* lisenter : Listeners)
		lisenter->OnTurnAction(ta);
}

void BattleEvent::DispatchCharacterTurnChange(std::string ta)
{
	for (auto* lisenter : Listeners)
		lisenter->OnCharacterTurnChange(ta);
}
