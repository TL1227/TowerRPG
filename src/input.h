#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "inputevent.h"
#include "battlesystem.h"

class Input
{
public:
    Input(GLFWwindow* window, InputEvent* inputEvent, BattleSystem* battleSystem);
    void Read();
    InputEvent* Event;
    GLFWwindow* Window;
    BattleSystem* BattleSystem;
    bool InputActionState[20];
private:
    void RegisterActionPress(InputAction);
    void RegisterActionKeyUp(InputAction);
};

#endif
