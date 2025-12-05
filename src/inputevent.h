#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "inputeventlistener.h"
#include <vector>

class InputEvent
{
public:
	void AddListener(InputEventListener&);
	void DispatchButtonPress(InputAction button);

private:
	std::vector<InputEventListener*> Listeners;
};

#endif
