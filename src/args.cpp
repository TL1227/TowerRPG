#include "args.h"

#include <string>

void Args::Parse(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "--live-edit") 
			IsLiveEdit = true;
		if (std::string(argv[i]) == "--map-path") 
			MapPath = argv[i + 1];
	}
}

Args G_Args;
