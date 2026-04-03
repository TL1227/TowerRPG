#include "menu.h"

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
