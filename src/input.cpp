#include "input.h"
#include "movementsystem.h"

#include "inputaction.h"

Input::Input(GLFWwindow* window, ::MovementSystem& movementSystem)
	: Window{ window }
	, MovementSystem { movementSystem }
	, CurrentReciever { &MovementSystem } //TODO: this will probably become a stack or something
{

}

void Input::Read()
{
	if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_D))
		RegisterActionPress(InputAction::Right);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_A))
		RegisterActionPress(InputAction::Left);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_RIGHT))
		RegisterActionPress(InputAction::StepRight);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_LEFT))
		RegisterActionPress(InputAction::StepLeft);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_DOWN))
		RegisterActionPress(InputAction::TurnAround);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_W))
		RegisterActionPress(InputAction::Up);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_S))
		RegisterActionPress(InputAction::Down);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_ENTER))
		RegisterActionPress(InputAction::Confirm);
	else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_BACKSPACE))
		RegisterActionPress(InputAction::Cancel);

	if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_D))
		RegisterActionKeyUp(InputAction::Right);
	if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_A))
		RegisterActionKeyUp(InputAction::Left);
	if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_RIGHT))
		RegisterActionKeyUp(InputAction::StepRight);
	if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_LEFT))
		RegisterActionKeyUp(InputAction::StepLeft);
	if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_DOWN))
		RegisterActionKeyUp(InputAction::TurnAround);
	if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_W))
		RegisterActionKeyUp(InputAction::Up);
	if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_S))
		RegisterActionKeyUp(InputAction::Down);
	if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_ENTER))
		RegisterActionKeyUp(InputAction::Confirm);
	if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_BACKSPACE))
		RegisterActionKeyUp(InputAction::Cancel);
}

void Input::RegisterActionPress(InputAction action)
{
	if (!InputActionState[(int)action])
	{
		CurrentReciever->RecieveInput(action);
		InputActionState[(int)action] = true;
	}
	else if (CurrentReciever->InputRepeats())
	{
		CurrentReciever->RecieveInput(action);
	}
}

void Input::RegisterActionKeyUp(InputAction action)
{
	if (InputActionState[(int)action])
	{
		InputActionState[(int)action] = false;
	}
}
