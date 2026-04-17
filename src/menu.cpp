#include "menu.h"
#include "inputaction.h"

Menu::Menu(std::vector<MenuItem> items) 
	: Items{ items } { } 

void Menu::SelectNextItem()
{
    if (++CurrentItemIndex >= Items.size()) 
    {
        CurrentItemIndex = 0;
    }
}

void Menu::SelectPreviousItem()
{
    if (--CurrentItemIndex < 0)
    {
        CurrentItemIndex = Items.size() - 1;
    }
}

MenuItem* Menu::GetCurrentSelection()
{
    return &Items[CurrentItemIndex];
}

void Menu::RecieveInput(InputAction IA)
{
	switch (IA)
	{
	case InputAction::Confirm:
            GetCurrentSelection()->Action();
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

bool Menu::InputRepeats() { return false; };
