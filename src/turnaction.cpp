#include "turnaction.h"

TurnAction::TurnAction(std::string name, float damagePoints, Side target)
: Name { name }, DamagePoints { damagePoints }, Target { target }
{

}

bool TurnAction::IsFinished()
{
    return Elapsed >= ActionTime;
}
