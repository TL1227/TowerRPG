#ifndef MENULIST_H
#define MENULIST_H

#include <string>
#include <vector>

class MenuList
{
public:
    std::string GetCurrentString();
    size_t GetCurrentIndex();
    bool Increment();
    bool AtEndOfList();
private:
    MenuList(std::vector<std::string> list);
    int Index = 0;
	std::vector<std::string> List;
    size_t Length = List.size() - 1;

    std::string CurrentPartyMember = List[Index];
};

#endif
