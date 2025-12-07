#include "inputevent.h"

void InputEvent::AddListener(InputEventListener& listener)
{
    Listeners.push_back(&listener);
}

void InputEvent::DispatchButtonPress(InputAction button)
{
    for (auto* l : Listeners)
        l->OnButtonPress(button);
}

void InputEvent::DispatchButtonPress(MenuAction button)
{
    for (auto* l : Listeners)
        l->OnMenuActionButtonPress(button);
}

void InputEvent::DispatchButtonPress(MoveAction button)
{
    for (auto* l : Listeners)
        l->OnMoveActionButtonPress(button);
}
