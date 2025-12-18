#ifndef VECTORSTRINGLIST_H
#define VECTORSTRINGLIST_H

#include <string>
#include <vector>

class VectorStringList
{
public:
    std::string GetCurrentString();
    size_t GetCurrentIndex();
    bool Increment();
    bool AtEndOfList();
private:
    VectorStringList(std::vector<std::string> list);
    int Index = 0;
	std::vector<std::string> List;
    size_t Length = List.size() - 1;

    std::string CurrentPartyMember = List[Index];
};

#endif
