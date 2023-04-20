#include "Window.hpp"

#include <Controller/Events/KeyEvent.hpp>
#include <Controller/Events/MouseEvent.hpp>
#include <Controller/Events/WindowEvent.hpp>

using namespace Vakol::Controller;

void APIENTRY DebugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
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
    }
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         VK_CRITICAL("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       VK_ERROR("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          VK_WARN("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: VK_TRACE("Severity: notification"); break;
    }

    std::cout << std::endl;
}

namespace Vakol::View {
    Window::Window(const std::string& title, int width, int height)
        : m_window(nullptr), m_title(title), m_width(width), m_height(height) {
        VK_INFO("Creating window {0} ({1}, {2})", m_title, m_width, m_height);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        glfwWindowHint(GLFW_SAMPLES, 4);

        /* Initialize the library */
        if (!glfwInit()) return;

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
        if (!m_window) {
            VK_CRITICAL("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            Window* wp = (Window*)glfwGetWindowUserPointer(window);
            wp->m_height = height;
            wp->m_width = width;

            WindowResizeEvent event(width, height);
            wp->m_eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
            Window* wp = (Window*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            wp->m_eventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            Window* wp = (Window*)glfwGetWindowUserPointer(window);

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

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            Window* wp = (Window*)glfwGetWindowUserPointer(window);

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

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            Window* wp = (Window*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            wp->m_eventCallback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
            Window* wp = (Window*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            wp->m_eventCallback(event);
        });
    }

    Window::~Window() { glfwDestroyWindow(m_window); }

    void Window::OnUpdate() {
        /* Swap front and back buffers */
        glfwSwapBuffers(this->m_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLFWwindow* Window::GetWindow() { return m_window; }

    void Window::SetEventCallback(const EventCallbackFn& callback) { m_eventCallback = callback; }
}  // namespace Vakol::View