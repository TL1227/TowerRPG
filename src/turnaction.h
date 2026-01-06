#ifndef TURNACTION_H
#define TURNACTION_H

#include <string>

enum class Side
{
    Party,
    Enemy
};

class TurnAction
{
public:
    TurnAction() = default;
    TurnAction(std::string name, float damagePoints, Side target);
    bool IsFinished();

    //inlcuded in datafile
    std::string Name;
    float DamagePoints = 0;
    Side Target = Side::Enemy;

    //worked out after somehow. Possibly shouldn't be part of the class
    Side User = Side::Party;
    float ActionTime = 1;
    float Elapsed = 0;
    float DamagePercent = 0;
};

#endif
