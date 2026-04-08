#include "battlemenu.h"

#include "inputaction.h"

void BattleMenu::RecieveInput(InputAction IA)
{
	switch (IA)
	{
	case InputAction::Confirm:
            HandleSelection();
		break;
	case InputAction::Up:
            SelectPreviousItem();
		break;
	case InputAction::Down:
            SelectNextItem();
		break;

	case InputAction::Cancel:
	case InputAction::None:
	case InputAction::Left:
	case InputAction::Right:
	default:
		break;
	}
}

void BattleMenu::Attack()
{

}

void BattleMenu::Skill()
{

}

void BattleMenu::Item()
{

}

void BattleMenu::Run()
{

}

BattleMenu::BattleMenu()
{
    Items = { 
        { "Attack", [this](){ Attack(); } },
        { "Skill",  [this](){ Skill();  } },
        { "Item",   [this](){ Item();   } },
        { "Run",    [this](){ Run();    } }
    };
}

void BattleMenu::HandleSelection()
{
    Items[CurrentItemIndex].Action();
}
