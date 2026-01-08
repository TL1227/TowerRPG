#include "skillparse.h"

#include <fstream>
#include <iostream>

SkillParse::SkillParse()
{
	std::ifstream file{SkillDataPath};

    if (!file)
    {
        std::cout << "Error Loading " << SkillDataPath << std::endl;
        return;
    }

    //TODO: I was tired and this is a mess, we'll fix later

    std::string line;
    std::vector<std::string> elements = { "", "", "", "" };
    int e = 0;
	while (std::getline(file, line))
	{
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] == ',')
            {
                e++;
                continue;
            }

            elements[e].push_back(line[i]);
        }

		TurnAction ta;
		ta.Name = elements[0];
        ta.DamagePoints = std::stoi(elements[1]);
        ta.Target = (Side)std::stoi(elements[2]);
        ta.Member = (PartyMember)std::stoi(elements[3]);

        PartySkills[ta.Member].push_back(ta);

        //reset counter and clear strings
        for (auto& el : elements) 
            el = "";

        e = 0;
	}
}

std::vector<TurnAction> SkillParse::GetSkills(int partyEnum)
{
    return PartySkills[(PartyMember)partyEnum];
}
