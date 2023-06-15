#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Window.hpp>
#include <Core/Log.hpp>
#include <Config.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Core::Window::Window()
{
	Core::Log* _log = Core::Log::instance();

	//	GLFW: initialize and configure

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_log->write("Creating GLFW Window");

	m_width = SCR_WIDTH;
	m_height = SCR_HEIGHT;

	m_widthf = (float)SCR_WIDTH;
	m_heightf = (float)SCR_HEIGHT;

	m_windowCoef = (float)m_height / (float)m_width;

	//	GLFW window creation
	m_window = glfwCreateWindow(m_width, m_height, "FPS - OpenGL", NULL, NULL);

	if (m_window == NULL)
	{
		_log->writeFailure("Failed to create GLFW window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		_log->writeFailure("Failed to initialize GLAD");
		return;
	}

}

void Core::Window::update()
{
	int oldW = m_width;
	int oldH = m_height;

	glfwGetWindowSize(m_window, &m_width, &m_height);

	m_widthf = (float)m_width;
	m_heightf = (float)m_height;

	m_wasReshaped = (m_width != oldW) || (m_height != oldH);

	if (m_wasReshaped)
	{
		m_windowCoef = (float)m_height / (float)m_width;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
