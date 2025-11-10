#ifndef ARGS_H
#define ARGS_H

class Args
{
public:
	bool IsLiveEdit = false;
	void Parse(int argc, char* argv[]);
};

#endif

