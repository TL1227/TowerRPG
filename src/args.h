#ifndef ARGS_H
#define ARGS_H

#include <string>

class Args
{
public:
	bool IsLiveEdit = false;
	bool BattleOff = false;
    std::string MapPath = {};
	void Parse(int argc, char* argv[]);
};

extern Args G_Args;

#endif

