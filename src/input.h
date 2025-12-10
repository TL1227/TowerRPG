#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "inputevent.h"
#include "battlesystem.h"
#include "menuaction.h"
#include "moveaction.h"

class Input
{
public:
    Input(GLFWwindow* window, InputEvent* inputEvent, BattleSystem* battleSystem);
    void Read();
private:
    bool InputActionState[20];
    bool MoveActionState[20];
    bool MenuActionState[20];
    BattleSystem* BattleSystem;
    InputEvent* Event;
    GLFWwindow* Window;
    void RegisterActionPress(InputAction);
    void RegisterActionKeyUp(InputAction);
    void RegisterActionPress(MoveAction);
    void RegisterActionKeyUp(MoveAction);
    void RegisterActionPress(MenuAction);
    void RegisterActionKeyUp(MenuAction);
};

#endif
