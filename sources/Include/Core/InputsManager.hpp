#pragma once

#include <unordered_map>
#include <Utils/Singleton.h>

#include <Maths/Vector2.h>

struct GLFWwindow;

enum InputList
{
	DEBUG_F1,
	DEBUG_F2,
	DEBUG_F3,
	DEBUG_F4,

	RIGHT_INPUT,
	LEFT_INPUT,

	FORWARD_INPUT,
	BACKWARD_INPUT,

	UP_INPUT,
	DOWN_INPUT,

	CAMERA_UP,
	CAMERA_DOWN,
	CAMERA_RIGHT,
	CAMERA_LEFT,

	RIGHT_CLICK,
	LEFT_CLICK,

	SPEED_UP,
	SPEED_DOWN,

	PAUSE,

	RELOAD,

	INPUT_COUNT
};


namespace Core
{

	struct InputKey
	{
		InputKey(int id)
		{
			keyID = id;

			released = false;
			pressed = false;
			down = false;
			up = false;
		}

		int keyID;
		bool pressed;
		bool released;
		bool down;
		bool up;

		virtual void getState(GLFWwindow* in_window);
	};

	struct MouseButton : public InputKey 
	{
		MouseButton(int id) : InputKey(id) {};

		void getState(GLFWwindow* in_window) override;
	};

	class InputsManager : public Singleton<InputsManager>
	{

	private:

		//	Private Internal Variables
		//	--------------------------

		bool m_isInitialized = false;

		double m_oldMouseX = 0.f, m_oldMouseY = 0.f;
		double m_newMouseX = 0.f, m_newMouseY = 0.f;


	public:

		//	Constructor & Destructor
		//	------------------------

		InputsManager() = default;
		~InputsManager();

		//	Public Internal Variables
		//	-------------------------

		std::unordered_map<int, InputKey*> m_key;

		float m_mouseDeltaX = 0.f, m_mouseDeltaY = 0.f;

		//	Public Internal Functions
		//	-------------------------

		void init();

		void updateInputs();
		void resetOldMouse();
		void hideMouse();
		void displayMouse();
		Maths::Vector2f getMousePosition();
	};
}