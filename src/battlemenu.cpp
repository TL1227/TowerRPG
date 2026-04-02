#include "battlemenu.h"

#include "inputaction.h"

void BattleMenu::RecieveInput(InputAction IA)
{
	switch (IA)
	{
	case InputAction::None:
		break;
	case InputAction::Confirm:
		break;
	case InputAction::Cancel:
		break;
	case InputAction::Up:
		SelectPreviousItem();
		break;
	case InputAction::Down:
		SelectNextItem();
		break;
	case InputAction::Left:
		break;
	case InputAction::Right:
		break;
	default:
		break;
	}
}
