#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "inputevent.h"
#include "menuaction.h"
#include "moveaction.h"

class BattleSystem;
class MovementSystem;
class InputReciever;

class Input
{
public:
    Input(GLFWwindow* window, MovementSystem& movementSystem);
    void Read();
private:
    bool InputActionState[20];
    BattleSystem* BattleSystem;
    MovementSystem& MovementSystem;
    InputEvent* Event;
    GLFWwindow* Window;
    InputReciever* CurrentReciever;
    void RegisterActionPress(InputAction);
    void RegisterActionKeyUp(InputAction);
};

#endif
