#include "menulist.h"

MenuList::MenuList(std::vector<std::string> list)
: List { list }
{

}

size_t MenuList::GetCurrentIndex()
{
    return Index;
}
std::string MenuList::GetCurrentString()
{
    return List[Index];
}

bool MenuList::AtEndOfList()
{
    return Index = Length;
}

//returns false if already at end of list
bool MenuList::Increment()
{
    if (AtEndOfList())
    {
        return false;
    }
    else
    {
        Index++;
        return true;
    }
}
