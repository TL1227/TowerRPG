#ifndef MENU_H
#define MENU_H

#include <vector>
#include "menuitem.h"
#include "InputReciever.h"

class MenuItem;

class Menu : InputReciever
{
public:
	Menu(std::vector<MenuItem> Items);
	void SelectNextItem();
	void SelectPreviousItem();
	MenuItem* GetCurrentSelection();

private:
	std::vector<MenuItem> Items;
	int CurrentItemIndex = 0;
	virtual void RecieveInput(InputAction) override;
};

#endif

