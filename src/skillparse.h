#ifndef SKILLPARSE_H
#define SKILLPARSE_H

#include "turnaction.h"

#include <vector>
#include <unordered_map>

class SkillParse
{
public:
    SkillParse();
    std::vector<TurnAction> GetSkills(int partyEnum);
private:
    std::string SkillDataPath = "data/skilldata";
    std::unordered_map<PartyMember, std::vector<TurnAction>> PartySkills;
};

#endif
