#include "args.h"

#include <string>

void Args::Parse(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "--live-edit") 
			IsLiveEdit = true;
		if (std::string(argv[i]) == "--load-map") 
			MapPath = argv[i + 1];
		if (std::string(argv[i]) == "--battle-off") 
            BattleOff = true;
	}
}

Args G_Args;
