#include "input.h"

#include "inputaction.h"

Input::Input(GLFWwindow* window, InputEvent* event, ::BattleSystem* battleSystem)
	: Window{ window }
	, Event{ event }
	, BattleSystem{ battleSystem }
{

}

void Input::Read()
{
    if (BattleSystem->GetPhase() == BattlePhase::Sighting || BattleSystem->GetPhase() == BattlePhase::End)
    {
		if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_D))
		{
			Event->DispatchButtonPress(InputAction::TurnRight);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_A))
		{
			Event->DispatchButtonPress(InputAction::TurnLeft);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_RIGHT))
		{
			Event->DispatchButtonPress(InputAction::MoveRight);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_LEFT))
		{
			Event->DispatchButtonPress(InputAction::MoveLeft);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_DOWN))
		{
			Event->DispatchButtonPress(InputAction::TurnAround);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_W))
		{
			Event->DispatchButtonPress(InputAction::MoveForwards);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_S))
		{
			Event->DispatchButtonPress(InputAction::MoveBackwards);
		}
    }
	else
	{
		if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_D))
			RegisterActionPress(InputAction::MenuRight);
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_A))
			RegisterActionPress(InputAction::MenuLeft);
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_W))
			RegisterActionPress(InputAction::MenuUp);
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_S))
			RegisterActionPress(InputAction::MenuDown);
		else if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_S))
			RegisterActionKeyUp(InputAction::MenuDown);
		else if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_D))
			RegisterActionKeyUp(InputAction::MenuRight);
		else if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_A))
			RegisterActionKeyUp(InputAction::MenuLeft);
		else if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_W))
			RegisterActionKeyUp(InputAction::MenuUp);
	}
}

void Input::RegisterActionPress(InputAction action)
{
	if (!InputActionState[(int)action])
	{
		Event->DispatchButtonPress(action);
		InputActionState[(int)action] = true;
	}
}

void Input::RegisterActionKeyUp(InputAction action)
{
	if (InputActionState[(int)action])
	{
		InputActionState[(int)action] = false;
	}
}
