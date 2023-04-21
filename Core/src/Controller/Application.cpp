#include "Application.hpp"

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/LuaAccess.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Controller/System.hpp>
#include <Model/Assets/Texture.hpp>
#include <Model/Components.hpp>
#include <View/Renderer/RendererFactory.hpp>

#include "Logger.hpp"

namespace Vakol::Controller {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() : m_running(false), m_window(nullptr), m_renderer(nullptr) { Logger::Init(); };

    void Application::Init() {
        Controller::RegisterLogger(lua.GetState());
        Controller::RegisterMath(lua.GetState());
        Controller::RegisterEntity(lua.GetState());
        Controller::RegisterECS(lua.GetState());
        Controller::RegisterAssetLoader(lua.GetState());
        Controller::RegisterApplication(lua.GetState(), this);
        Controller::RegisterScene(lua.GetState());
        Controller::RegisterGUIWindow(lua.GetState(), &m_gui);  // Register GUI Window

        // void RegisterRenderer(sol::state& lua);
        // void RegisterPhysics(sol::state& lua);

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

        // Physics::Debug = false;

        // Physics::Init();

        VK_INFO("Calling main.lua...");

        lua.RunFile("scripts/main.lua");

        sol::function luaMain = lua.GetState()["main"];

        luaMain();

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
        if (!windowWidth) {
            VK_ERROR("CONFIG ERROR: Window Width Not set");
            return std::nullopt;
        }
        sol::optional<int> windowHeight = config["window"]["h"];
        if (!windowHeight) {
            VK_ERROR("CONFIG ERROR: Window Height Not Set");
            return std::nullopt;
        }
        sol::optional<std::string> rendererType = config["renderer"];
        if (!rendererType) {
            VK_ERROR("CONFIG ERROR: Renderer Not Set");
            return std::nullopt;
        }

        sol::optional<std::string> modelDir = config["model_dir"];
        if (!modelDir) {
            VK_WARN("CONFIG WARNING: No Model Directory Set, Using Default {0}", AssetLoader::model_path);
        } else {
            AssetLoader::model_path = modelDir.value();
        }

        sol::optional<std::string> textureDir = config["texture_dir"];
        if (!textureDir) {
            VK_WARN("CONFIG WARNING: No Texture Directory Set, Using Default {0}", AssetLoader::texture_path);
        } else {
            AssetLoader::texture_path = textureDir.value();
        }

        sol::optional<std::string> shaderDir = config["shader_dir"];
        if (!shaderDir) {
            VK_WARN("CONFIG WARNING: No Shader Directory Set, Using Default {0}", AssetLoader::shader_path);
        } else {
            AssetLoader::shader_path = shaderDir.value();
        }

        Model::GameConfig cfg = {name.value(), windowWidth.value(), windowHeight.value(), rendererType.value()};

        return cfg;
    }

    Application::~Application() {
        // json::SerializeScene("assets/json/Deserialize.json", m_entityList);
    }

    void Application::Run() {
        while (m_running) {
            m_time.Update();
            m_input.Update();
            m_gui.CreateNewFrame();

            // Physics::FixedUpdate(m_time, m_entityList.GetEntityList());

            m_renderer->Update();

            //! update scenes lua
            for (auto& scene : scenes) {
                System::BindScene(scene);
                scene.Update(m_time, m_renderer);
            }

            m_gui.Update();

            m_window->OnUpdate();
        }
    }

    void Application::AddScene(std::string scriptName, std::string scene_name) {
        std::string sceneName = scene_name.length() == 0 ? "Scene" + std::to_string(scenes.size()) : scene_name;

        scenes.push_back(Scene(sceneName, scriptName, lua,
                               std::make_shared<Physics::ScenePhysics>(Physics::PhysicsPool::CreatePhysicsWorld()),
                               true));
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
        if (kev.GetKeyCode() == GLFW_KEY_ESCAPE) {
            m_running = false;
            return true;
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