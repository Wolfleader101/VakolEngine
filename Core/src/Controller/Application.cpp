#include "Application.hpp"

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Controller/System.hpp>
#include <Model/Components.hpp>
#include <View/Renderer/RendererFactory.hpp>

#include "Logger.hpp"

namespace Vakol::Controller {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() : m_window(nullptr), m_renderer(nullptr), m_running(false), m_input(), m_scriptEngine() {
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

        if (m_window == nullptr) {
            return;
        }

        m_renderer = CreateRenderer(config.value().rendererType, m_window);

        if (m_renderer == nullptr) {
            return;
        }

        m_window->SetEventCallback([this](auto&& PH1) { OnEvent(std::forward<decltype(PH1)>(PH1)); });

        m_gui.Init(m_window);

        VK_INFO("Calling main.lua...");

        LuaScript mainScript = m_scriptEngine.CreateScript("scripts/main.lua");

        m_running = true;
    }

    //! this will be yeeted once script engine is done
    void Application::RegisterLua() {
        m_scriptEngine.SetGlobalVariable("Time", &m_time);
        m_scriptEngine.SetGlobalVariable("Input", &m_input);
        m_scriptEngine.SetGlobalVariable("GUI", &m_gui);

        m_scriptEngine.SetGlobalFunction("app_run", &Application::SetRunning, this);
        m_scriptEngine.SetGlobalFunction("add_scene", &Application::AddScene, this);
        m_scriptEngine.SetGlobalFunction("get_scene", &Application::GetScene, this);
        m_scriptEngine.SetGlobalFunction("set_active_mouse", &Application::SetActiveMouse, this);

        m_scriptEngine.SetGlobalFunction("toggle_wireframe", &View::Renderer::ToggleWireframe, m_renderer);
        m_scriptEngine.SetGlobalFunction("toggle_skybox", &View::Renderer::ToggleSkybox, m_renderer);
        m_scriptEngine.SetGlobalFunction("set_wireframe", &View::Renderer::SetWireframe, m_renderer);
        m_scriptEngine.SetGlobalFunction("set_skybox", &View::Renderer::SetSkybox, m_renderer);

        // lua.set_function("clear_color_v", [&](const glm::vec4& color) { renderer->ClearColor(color); });

        // lua.set_function("clear_color", [&](const float r, const float g, const float b, const float a) {
        //     renderer->ClearColor(r, g, b, a);
        //     renderer->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // });
    }

    std::optional<Model::GameConfig> Application::LoadConfig() {
        VK_INFO("Loading game_config.lua...");

        LuaScript configScript = m_scriptEngine.CreateScript("scripts/game_config.lua");

        sol::table config = m_scriptEngine.GetScriptVariable(configScript, "game_config");

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

    void Application::Run() {
        while (m_running) {
            m_time.Update();
            m_gui.CreateNewFrame();

            m_renderer->Update();

            m_time.accumulator += m_time.deltaTime;

            while (m_time.accumulator >= m_time.tickRate) {
                // eventually need to do something like m_scenemanager.GetCurrentScene().GetScript()

                for (auto& scene : scenes) {
                    if (!scene.active) continue;

                    //! set the current scene globally, eventually want to move this elsewhere
                    m_scriptEngine.SetGlobalVariable("scene", &scene);

                    if (!scene.initialized) {
                        m_scriptEngine.InitScript(scene.GetScript());

                        scene.Init();
                    }

                    m_scriptEngine.TickScript(scene.GetScript());
                }

                // Decrease the accumulated time
                m_time.accumulator -= m_time.tickRate;
            }

            // Compute the time interpolation factor
            // float alpha = m_time.accumulator / m_time.tickRate;

            // TODO move to a scene manager
            for (auto& scene : scenes) {
                if (!scene.active) continue;

                System::BindScene(scene);

                m_renderer->UpdateData(scene.GetCamera());

                //! set the current scene globally, eventually want to move this elsewhere
                m_scriptEngine.SetGlobalVariable("scene", &scene);

                scene.GetEntityList().GetRegistry().view<LuaScript>().each(
                    [&](auto& script) { m_scriptEngine.UpdateScript(script); });

                m_scriptEngine.UpdateScript(scene.GetScript());

                scene.Update(m_time, m_renderer);
            }

            m_renderer->LateUpdate();

            m_gui.Update();
            m_input.Update();
            m_window->OnUpdate();
        }
    }

    // TODO move to a scene manager
    void Application::AddScene(const std::string& scriptName, const std::string& scene_name) {
        const std::string sceneName = scene_name.length() == 0 ? "Scene" + std::to_string(scenes.size()) : scene_name;

        const auto ref = std::make_shared<ScenePhysics>(PhysicsPool::CreatePhysicsWorld());

        LuaScript script = m_scriptEngine.CreateScript("scripts/" + scriptName);
        LuaTable tbl = m_scriptEngine.CreateTable();
        m_scriptEngine.SetScriptVariable(script, "globals", tbl);
        scenes.emplace_back(sceneName, script, ref, true, m_scriptEngine);
    }

    Scene& Application::GetScene(const std::string& sceneName) {
        for (auto& scene : scenes) {
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
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleased));
    }

    void Application::SetActiveMouse(const bool active) {
        if (active)
            glfwSetInputMode(m_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(m_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& ev) {
        m_running = false;
        return true;
    }

    bool Application::OnWindowResize(const WindowResizeEvent& ev) const {
        glViewport(0, 0, ev.GetWidth(), ev.GetHeight());

        return true;
    }

    bool Application::OnKeyPressed(KeyPressedEvent& kev) {
        if (kev.GetKeyCode() == GLFW_KEY_K) m_renderer->ToggleWireframe();

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

    bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& mev) {
        m_input.OnMouseButtonPressed(mev);

        return true;
    }

    bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& mev) {
        m_input.OnMouseButtonReleased(mev);

        return true;
    }

    bool Application::IsRunning() const { return m_running; }
}  // namespace Vakol::Controller