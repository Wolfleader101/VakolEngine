#include "Window.hpp"
#include <Controller/Events/WindowEvent.hpp>
#include <Controller/Events/KeyEvent.hpp>
#include <Controller/Events/MouseEvent.hpp>

using namespace Vakol::Controller;

namespace Vakol::View
{
	Window::Window(const std::string& title, int width, int height): m_window(nullptr), m_title(title), m_width(width), m_height(height)
	{
		VK_INFO("Creating window {0} ({1}, {2})", m_title, m_width, m_height);

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


		/* Initialize the library */
		if (!glfwInit())
			return;

		/* Create a windowed mode window and its OpenGL context */
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
		if (!m_window)
		{
			VK_CRITICAL("Failed to create GLFW window");
			glfwTerminate();
			return;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(m_window);
		//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			VK_CRITICAL("Failed to initialise GLAD");
			return;
		}

		glfwSetWindowUserPointer(m_window, this);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				Window* wp = (Window*)glfwGetWindowUserPointer(window);
				wp->m_height = height;
				wp->m_width = width;

				WindowResizeEvent event(width, height);
				wp->m_eventCallback(event);

			});


		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				Window* wp = (Window*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				wp->m_eventCallback(event);

			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Window* wp = (Window*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					wp->m_eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					wp->m_eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					wp->m_eventCallback(event);
					break;
				}
				default:
					break;
				}
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				Window* wp = (Window*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					wp->m_eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					wp->m_eventCallback(event);
					break;
				}
				default:
					break;
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				Window* wp = (Window*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				wp->m_eventCallback(event);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				Window* wp = (Window*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				wp->m_eventCallback(event);
			});
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_window);
	}

	void Window::OnUpdate()
	{
        /* Swap front and back buffers */
        glfwSwapBuffers(this->m_window);

        /* Poll for and process events */
        glfwPollEvents();

	}

    GLFWwindow* Window::GetWindow()
    {
        return m_window;
    }
    
	void Window::SetEventCallback(const EventCallbackFn& callback)
    {
        m_eventCallback = callback;
    }
}