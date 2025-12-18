#ifndef TURNACTION_H
#define TURNACTION_H

#include <string>

class TurnAction
{
public:
    TurnAction() = default;
    bool IsFinished();
    std::string Name;
    float DamagePercent = 0;
    float DamagePoints = 0;
    bool TargetsEnemy = false;
    float ActionTime = 0;
    float Elapsed = 0;
};

#endif
