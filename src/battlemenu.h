#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include "menu.h"

class BattleMenu : public Menu
{
public:
    BattleMenu();
    void Attack();
    void Skill();
    void Item();
    void Run();

private:
	virtual void RecieveInput(InputAction) override;
    void HandleSelection();
};

#endif

