#include <Core/InputsManager.hpp>
#include <Core/Window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core
{
	InputsManager::~InputsManager()
	{
		for (auto key : m_key)
		{
			delete key.second;
		}
		m_key.clear();
	}


	void InputsManager::init()
	{
		if (m_isInitialized) return;

		m_isInitialized = true;

		//	X AXIS MOVEMENTS
		m_key[RIGHT_INPUT] = new  InputKey(GLFW_KEY_D);
		m_key[LEFT_INPUT] = new  InputKey(GLFW_KEY_A);
		
		//	Z AXIS MOVEMENTS
		m_key[FORWARD_INPUT] = new  InputKey(GLFW_KEY_W);
		m_key[BACKWARD_INPUT] = new  InputKey(GLFW_KEY_S);

		//	Y AXIS MOVEMENTS
		m_key[UP_INPUT] = new  InputKey(GLFW_KEY_SPACE);
		m_key[DOWN_INPUT] = new  InputKey(GLFW_KEY_LEFT_SHIFT);

		// X AND Z AXIS CAMERA ROTATION
		m_key[CAMERA_UP] = new  InputKey(GLFW_KEY_UP);
		m_key[CAMERA_DOWN] = new  InputKey(GLFW_KEY_DOWN);

		// Y AXIS CAMERA ROTATION
		m_key[CAMERA_RIGHT] = new  InputKey(GLFW_KEY_RIGHT);
		m_key[CAMERA_LEFT] = new  InputKey(GLFW_KEY_LEFT);

		//	MOUSE BUTTONS
		m_key[RIGHT_CLICK] = new  MouseButton(GLFW_MOUSE_BUTTON_2);
		m_key[LEFT_CLICK] = new  MouseButton(GLFW_MOUSE_BUTTON_1);

		//	SPEED BUTTONS
		m_key[SPEED_UP] = new  InputKey(GLFW_KEY_E);
		m_key[SPEED_DOWN] = new  InputKey(GLFW_KEY_Q);

		//	PAUSE BUTTON
		m_key[PAUSE] = new  InputKey(GLFW_KEY_ESCAPE);

		//	DEBUG INPUTS
		m_key[DEBUG_F1] = new  InputKey(GLFW_KEY_F1);
		m_key[DEBUG_F2] = new  InputKey(GLFW_KEY_F2);
		m_key[DEBUG_F3] = new  InputKey(GLFW_KEY_F3);
		m_key[DEBUG_F4] = new  InputKey(GLFW_KEY_F4);

		m_key[RELOAD] = new  InputKey(GLFW_KEY_R);
	}


	void InputsManager::updateInputs()
	{
		GLFWwindow* window = Core::Window::instance()->m_window;
		for (auto key : m_key)
		{
			key.second->getState(window);
		}

		glfwGetCursorPos(window, &m_newMouseX, &m_newMouseY);

		m_mouseDeltaX = (float)(m_newMouseX - m_oldMouseX);
		m_mouseDeltaY = (float)(m_newMouseY - m_oldMouseY);

		m_oldMouseX = m_newMouseX;
		m_oldMouseY = m_newMouseY;
	}

	Maths::Vector2f InputsManager::getMousePosition()
	{
		double dx, dy;
		glfwGetCursorPos(Core::Window::instance()->m_window, &dx, &dy);

		return Maths::Vector2f((float)dx, (float)dy);
	}

	void InputsManager::resetOldMouse()
	{
		glfwGetCursorPos(Core::Window::instance()->m_window, &m_oldMouseX, &m_oldMouseY);
	}


	void InputsManager::hideMouse()
	{
		glfwSetInputMode(Core::Window::instance()->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}


	void InputsManager::displayMouse()
	{
		glfwSetInputMode(Core::Window::instance()->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}


	void InputKey::getState(GLFWwindow* in_window)
	{
		bool oldDown = down;
		bool oldUp = up;
		 
		down = glfwGetKey(in_window, keyID);

		up = !down;

		if (up && oldUp != up) released = true;
		else released = false;

		if (down && oldDown != down) pressed = true;
		else pressed = false;
	}


	void MouseButton::getState(GLFWwindow* in_window)
	{
		bool oldDown = down;
		bool oldUp = up;

		down = glfwGetMouseButton(in_window, keyID);

		up = !down;

		if (up && oldUp != up) released = true;
		else released = false;

		if (down && oldDown != down) pressed = true;
		else pressed = false;	
	}
}