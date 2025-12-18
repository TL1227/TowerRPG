#include "vectorstringlist.h"

VectorStringList::VectorStringList(std::vector<std::string> list)
: List { list }
{

}

size_t VectorStringList::GetCurrentIndex()
{
    return Index;
}
std::string VectorStringList::GetCurrentString()
{
    return List[Index];
}

bool VectorStringList::AtEndOfList()
{
    return Index = Length;
}

//returns false if already at end of list
bool VectorStringList::Increment()
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
