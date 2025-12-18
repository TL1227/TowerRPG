#include "turnaction.h"

bool TurnAction::IsFinished()
{
    return Elapsed >= ActionTime;
}
