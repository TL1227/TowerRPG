#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include <functional>

class MenuItem
{
public:
    std::string Label;
    std::function<void()> Action;
};

#endif

