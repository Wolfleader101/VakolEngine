#include "Window.hpp"

#include <Controller/Events/KeyEvent.hpp>
#include <Controller/Events/MouseEvent.hpp>
#include <Controller/Events/WindowEvent.hpp>

#include <iostream>
#include <Controller/Logger.hpp>

using namespace Vakol::Controller;

void APIENTRY DebugOutput(const unsigned int source, const unsigned int type, const unsigned int id, const unsigned int severity, [[maybe_unused]] int length, const char* message, [[maybe_unused]] const void* userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

    VK_TRACE("---------------");
    VK_TRACE("Debug message ({0})", message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             VK_TRACE("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   VK_TRACE("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: VK_TRACE("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     VK_TRACE("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     VK_TRACE("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           VK_TRACE("Source: Other"); break;
    default: 
        break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               VK_TRACE("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: VK_TRACE("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  VK_TRACE("Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         VK_TRACE("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         VK_TRACE( "Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              VK_TRACE( "Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          VK_TRACE( "Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           VK_TRACE( "Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               VK_TRACE( "Type: Other"); break;
    default: 
        break;
    }
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         VK_CRITICAL("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       VK_ERROR("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          VK_WARN("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: VK_TRACE("Severity: notification"); break;
    default: 
        break;
    }

    std::cout << std::endl;
}

namespace Vakol::View {
    Window::Window(std::string title, int width, int height)
        : m_window(nullptr), m_title(std::move(title)), m_width(width), m_height(height) {
        VK_INFO("Creating window {0} ({1}, {2})", m_title, m_width, m_height);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
        //glfwWindowHint(GLFW_SAMPLES, 4);

        /* Initialize the library */
        if (!glfwInit()) return;

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window) {
            VK_CRITICAL("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            VK_CRITICAL("Failed to initialise GLAD");
            return;
        }

        // enable OpenGL debug context if context allows for debug context
        int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
            //glDebugMessageCallback(DebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        glfwSetWindowUserPointer(m_window, this);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, const int width, const int height) {
	        const auto wp = static_cast<Window*>(glfwGetWindowUserPointer(window));
            wp->m_height = height;
            wp->m_width = width;

            WindowResizeEvent event(width, height);
            wp->m_eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
	        auto wp = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            wp->m_eventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, const int key, [[maybe_unused]] int scancode, const int action, [[maybe_unused]] int mods) {
	        const auto wp = static_cast<Window*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    wp->m_eventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    wp->m_eventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    wp->m_eventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, const int button, const int action, [[maybe_unused]] int mods) {
	        const auto wp = static_cast<Window*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    wp->m_eventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    wp->m_eventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, const double xOffset, const double yOffset) {
	        const auto wp = static_cast<Window*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            wp->m_eventCallback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, const double xPos, const double yPos) {
	        auto wp = static_cast<Window*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            wp->m_eventCallback(event);
        });
    }

    Window::~Window() { glfwDestroyWindow(m_window); }

    void Window::OnUpdate() const
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(this->m_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLFWwindow* Window::GetWindow() const { return m_window; }

    void Window::SetEventCallback(const EventCallbackFn& callback) { m_eventCallback = callback; }
}  // namespace Vakol::View