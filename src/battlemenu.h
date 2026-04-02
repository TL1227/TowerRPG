#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include "menu.h"

class BattleMenu : public Menu
{
	virtual void RecieveInput(InputAction) override;
};

#endif

