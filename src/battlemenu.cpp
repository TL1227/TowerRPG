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

BattleMenu::BattleMenu()
{
    Items = { "Attack", "Skill", "Item", "Run" };
}

void BattleMenu::HandleSelection()
{
    switch (CurrentItemIndex)
    {
        case 0: //Attack
            break;
        case 1: //Skill
            break;
        case 2: //Item
            break;
        case 3: //Run
            break;
    }
}
