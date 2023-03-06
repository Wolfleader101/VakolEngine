#include "Application.hpp"

#include <Controller/LuaAccess.hpp>
#include <View/Renderer/RendererFactory.hpp>

#include "Logger.hpp"

// #include "JSON/Json.hpp"
// #include "Physics/Physics.hpp"

namespace Vakol::Controller {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() : m_running(false), m_window(nullptr), m_renderer(nullptr) { Logger::Init(); };

    void Application::Init() {
        // Controller::RegisterGameConfig(lua.GetState());
        // load type of renderer
        // get window config
        // get scripts to load

        auto config = LoadConfig();
        if (!config) {
            VK_CRITICAL("CONFIG COULD NOT BE LOADED");
            return;
        }

        m_window = std::make_shared<View::Window>(config.value().name, config.value().windowWidth,
                                                  config.value().windowHeight);

        m_renderer = CreateRenderer(config.value().rendererType, m_window);

        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        // m_gui.Init(m_window.GetWindow());

        // Physics::Debug = false;

        // Physics::Init();

        m_running = true;
    }

    std::optional<Model::GameConfig> Application::LoadConfig() {
        VK_INFO("Loading game_config.lua...");

        lua.RunFile("scripts/game_config.lua");

        sol::table config = lua.GetState()["game_config"];

        sol::optional<std::string> name = config["name"];
        if (!name) {
            VK_ERROR("CONFIG ERROR: Game Name Not Set");
            return std::nullopt;
        }
        sol::optional<int> windowWidth = config["window"]["w"];
        sol::optional<int> windowHeight = config["window"]["h"];
        sol::optional<std::string> rendererType = config["renderer"];
    }

    Application::~Application() {
        // json::SerializeScene("assets/json/Deserialize.json", m_entityList);
    }

    void Application::Run() {
        while (m_running) {
            m_time.Update();

            //! update lua

            // Physics::FixedUpdate(m_time, m_entityList.GetEntityList());

            // for (Layer* layer : m_layerStack) layer->OnUpdate(m_time, m_entityList);

            // for (auto& ent : m_entityList.GetEntityList()) {
            //     if (ent.OnUpdate) ent.OnUpdate(m_time, ent);
            // }

            m_renderer->Update(m_time);
            // m_renderer.Update(m_time, m_entityList.GetEntityList());

            // m_gui.OnUpdate();

            m_window->OnUpdate();
        }

        // ImGui_ImplOpenGL3_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();
    }

    void Application::OnEvent(Event& ev) {
        EventDispatcher dispatcher(ev);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        //! lua on event

        // for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
        //     (*--it)->OnEvent(ev);
        //     if (ev.Handled) break;
        // }

        // VK_TRACE(ev);

        // iterate backwards in the layer stack
        // call on event
        // if its handled then u can break
    }

    bool Application::OnWindowClose(WindowCloseEvent& ev) {
        m_running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& ev) {
        glViewport(0, 0, ev.GetWidth(), ev.GetHeight());

        return true;
    }

    bool Application::OnKeyPressed(KeyPressedEvent& kev) {
        if (kev.GetKeyCode() == GLFW_KEY_ESCAPE) {
            exit(0);
            return true;  // wont be hit but compiler sad without
        }

        return true;
    }

    bool Application::IsRunning() const { return m_running; }
}  // namespace Vakol::Controller