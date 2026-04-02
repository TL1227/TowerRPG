#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

#include "InputReciever.h"

class Menu : public InputReciever
{
	int CurrentItemIndex = 0;
	std::vector<std::string> Items;
	virtual void RecieveInput(InputAction) = 0;

public:
	void SelectNextItem();
	void SelectPreviousItem();
};

#endif

