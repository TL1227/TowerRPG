#ifndef MENU_H
#define MENU_H

#include <vector>

#include "InputReciever.h"
#include "menuitem.h"

class MenuItem;

class Menu : public InputReciever
{
    virtual void RecieveInput(InputAction) = 0;

protected:
	int CurrentItemIndex = 0;
	std::vector<MenuItem> Items;

public:
	void SelectNextItem();
	void SelectPreviousItem();
};

#endif

