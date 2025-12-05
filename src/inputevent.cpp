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
