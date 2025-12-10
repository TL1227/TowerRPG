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
			Event->DispatchButtonPress(MoveAction::TurnRight);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_A))
		{
			Event->DispatchButtonPress(MoveAction::TurnLeft);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_RIGHT))
		{
			Event->DispatchButtonPress(MoveAction::Right);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_LEFT))
		{
			Event->DispatchButtonPress(MoveAction::Left);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_DOWN))
		{
			Event->DispatchButtonPress(MoveAction::TurnAround);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_W))
		{
			Event->DispatchButtonPress(MoveAction::Forwards);
		}
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_S))
		{
			Event->DispatchButtonPress(MoveAction::Backwards);
		}
    }
    else if (BattleSystem->GetPhase() == BattlePhase::Start)
	{
		if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_D))
			RegisterActionPress(MenuAction::Right);
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_A))
			RegisterActionPress(MenuAction::Left);
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_W))
			RegisterActionPress(MenuAction::Up);
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_S))
			RegisterActionPress(MenuAction::Down);
		else if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_ENTER))
			RegisterActionPress(MenuAction::Confirm);

		if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_S))
			RegisterActionKeyUp(MenuAction::Down);
		if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_D))
			RegisterActionKeyUp(MenuAction::Right);
		if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_A))
			RegisterActionKeyUp(MenuAction::Left);
		if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_W))
			RegisterActionKeyUp(MenuAction::Up);
		if (GLFW_RELEASE == glfwGetKey(Window, GLFW_KEY_ENTER))
			RegisterActionKeyUp(MenuAction::Confirm);
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

void Input::RegisterActionPress(MoveAction action)
{
	if (!MoveActionState[(int)action])
	{
		Event->DispatchButtonPress(action);
		MoveActionState[(int)action] = true;
	}
}

void Input::RegisterActionKeyUp(MoveAction action)
{
	if (MoveActionState[(int)action])
	{
		MoveActionState[(int)action] = false;
	}
}

void Input::RegisterActionPress(MenuAction action)
{
	if (!MenuActionState[(int)action])
	{
		Event->DispatchButtonPress(action);
		MenuActionState[(int)action] = true;
	}
}

void Input::RegisterActionKeyUp(MenuAction action)
{
	if (MenuActionState[(int)action])
	{
		MenuActionState[(int)action] = false;
	}
}
