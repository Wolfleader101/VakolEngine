#include "Application.hpp"

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/LuaAccess.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Controller/System.hpp>
#include <Model/Components.hpp>
#include <View/Renderer/RendererFactory.hpp>

#include "Logger.hpp"

namespace Vakol::Controller {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() : m_window(nullptr), m_renderer(nullptr), m_running(false) {
        Logger::Init();
        scenes.reserve(10);
    };

    void Application::Init() {
        RegisterLua();

        auto config = LoadConfig();

        if (!config) {
            VK_CRITICAL("CONFIG COULD NOT BE LOADED");
            return;
        }

        m_window = std::make_shared<View::Window>(config.value().name, config.value().windowWidth,
                                                  config.value().windowHeight);

        m_renderer = CreateRenderer(config.value().rendererType, m_window);

        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_gui.Init(m_window);

        VK_INFO("Calling main.lua...");

        lua.RunFile("scripts/main.lua");

        const sol::function lua_main = lua.GetState()["main"];

        m_running = true;
    }

    void Application::RegisterLua() {
        RegisterLogger(lua.GetState());
        RegisterMath(lua.GetState());
        RegisterEntity(lua, lua.GetState());
        RegisterECS(lua.GetState());
        RegisterAssetLoader(lua.GetState());
        RegisterApplication(lua.GetState(), this);
        RegisterScene(lua.GetState());
        RegisterGUIWindow(lua.GetState(), &m_gui);  // Register GUI Window
        RegisterRenderer(lua.GetState());
        RegisterPhysics(lua.GetState());
        RegisterOther(lua.GetState());
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
        sol::optional<int> window_width = config["window"]["w"];
        if (!window_width) {
            VK_ERROR("CONFIG ERROR: Window Width Not set");
            return std::nullopt;
        }
        sol::optional<int> window_height = config["window"]["h"];
        if (!window_height) {
            VK_ERROR("CONFIG ERROR: Window Height Not Set");
            return std::nullopt;
        }
        sol::optional<std::string> renderer_type = config["renderer"];
        if (!renderer_type) {
            VK_ERROR("CONFIG ERROR: Renderer Not Set");
            return std::nullopt;
        }

        if (sol::optional<std::string> model_dir = config["model_dir"]; !model_dir) {
            VK_WARN("CONFIG WARNING: No Model Directory Set, Using Default {0}", AssetLoader::model_path);
        } else {
            AssetLoader::model_path = model_dir.value();
        }

        if (sol::optional<std::string> texture_dir = config["texture_dir"]; !texture_dir) {
            VK_WARN("CONFIG WARNING: No Texture Directory Set, Using Default {0}", AssetLoader::texture_path);
        } else {
            AssetLoader::texture_path = texture_dir.value();
        }

        if (sol::optional<std::string> shader_dir = config["shader_dir"]; !shader_dir) {
            VK_WARN("CONFIG WARNING: No Shader Directory Set, Using Default {0}", AssetLoader::shader_path);
        } else {
            AssetLoader::shader_path = shader_dir.value();
        }

        Model::GameConfig cfg = {name.value(), window_width.value(), window_height.value(), renderer_type.value()};

        return cfg;
    }

    Application::~Application() {}

    void Application::Run() {
        while (m_running) {
            m_time.Update();
            m_gui.CreateNewFrame();

            m_renderer->Update();

            //! update scenes lua
            for (auto& scene : scenes) {
                if (!scene.active) continue;

                if (!scene.initialized) scene.Init();

                System::BindScene(scene);
                scene.Update(m_time, m_renderer);
            }

            m_gui.Update();
            m_input.Update();
            m_window->OnUpdate();
        }
    }

    void Application::AddScene(std::string scriptName, std::string scene_name) {
        std::string sceneName = scene_name.length() == 0 ? "Scene" + std::to_string(scenes.size()) : scene_name;

        scenes.push_back(Scene(sceneName, scriptName, lua,
                               std::make_shared<Physics::ScenePhysics>(Physics::PhysicsPool::CreatePhysicsWorld()),
                               true));
    }

    Scene& Application::GetScene(const std::string& sceneName)
    {
	    for (auto& scene : scenes)
	    {
	    	if (scene.getName() == sceneName) return scene;
		}

        VK_CRITICAL("Scene: {0} could not be found.", sceneName);
		assert(false);
    }



    void Application::OnEvent(Event& ev) {
        EventDispatcher dispatcher(ev);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMoved));

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

    bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& ev) {
        m_running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& ev) {
        glViewport(0, 0, ev.GetWidth(), ev.GetHeight());

        return true;
    }

    bool Application::OnKeyPressed(KeyPressedEvent& kev) {
        
    	if (kev.GetKeyCode() == GLFW_KEY_K) {
            m_renderer->ToggleWireframe();
        }

        m_input.OnKeyPressed(kev);

        return true;
    }

    bool Application::OnKeyReleased(KeyReleasedEvent& kev) {
        m_input.OnKeyReleased(kev);
        return true;
    }
    bool Application::OnMouseMoved(MouseMovedEvent& ev) {
        m_input.OnMouseMoved(ev);
        return true;
    }

    bool Application::IsRunning() const { return m_running; }
}  // namespace Vakol::Controller