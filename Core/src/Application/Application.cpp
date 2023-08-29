#include "Application/Application.hpp"

#include "GameConfig.hpp"
#include "Logger/Logger.hpp"

#include "Rendering/RenderEngine.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "ECS/Components.hpp"

namespace Vakol
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
        : m_window(nullptr), m_scriptEngine(), m_sceneManager(m_scriptEngine, m_physicsEngine), m_running(false),
          m_gameState(GameState::Running), m_activeSystems(0), m_input(){};

    void Application::Init()
    {
        RegisterLua();

        std::optional<GameConfig> config = LoadConfig();

        if (!config)
        {
            VK_CRITICAL("CONFIG COULD NOT BE LOADED");
            return;
        }

        m_window =
            std::make_shared<Window>(config.value().name, config.value().windowWidth, config.value().windowHeight);

        if (m_window == nullptr)
        {
            return;
        }

        m_window->SetEventCallback([this](auto&& PH1) { OnEvent(std::forward<decltype(PH1)>(PH1)); });

        Rendering::RenderEngine::Init(config.value().windowWidth, config.value().windowHeight,
                                      config.value().rendererType);

        m_gui.Init(m_window);

        VK_INFO("Calling main.lua...");

        LuaScript mainScript = m_scriptEngine.CreateScript("scripts/main.lua");

        m_running = true;

        m_activeSystems = static_cast<int>(SystemFlag::Scripting) | static_cast<int>(SystemFlag::Physics) |
                          static_cast<int>(SystemFlag::Rendering);
    }

    void Application::RegisterLua()
    {
        m_scriptEngine.SetGlobalVariable("Time", &m_time);
        m_scriptEngine.SetGlobalVariable("Input", &m_input);
        m_scriptEngine.SetGlobalVariable("GUI", &m_gui);

        m_scriptEngine.SetGlobalFunction("app_run", &Application::SetRunning, this);

        m_scriptEngine.SetGlobalFunction("add_scene", &SceneManager::CreateScene, &m_sceneManager);
        m_scriptEngine.SetGlobalFunction("get_scene", &SceneManager::GetScene, &m_sceneManager);
        m_scriptEngine.SetGlobalFunction("remove_scene", &SceneManager::RemoveScene, &m_sceneManager);
        m_scriptEngine.SetGlobalFunction("change_scene", &SceneManager::ChangeActiveScene, &m_sceneManager);
        // m_scriptEngine.SetGlobalFunction("set_current_scene", &SceneManager::SetactiveScene, &m_sceneManager);
        // dont think we need get_current_scene as that's more for backend

        m_scriptEngine.SetGlobalFunction("set_active_mouse", &Application::SetActiveMouse, this);

        // m_scriptEngine.SetGlobalFunction("toggle_wireframe", &Renderer::ToggleWireframe, m_renderer);
        // m_scriptEngine.SetGlobalFunction("toggle_skybox", &Renderer::ToggleSkybox, m_renderer);
        // m_scriptEngine.SetGlobalFunction("set_wireframe", &Renderer::SetWireframe, m_renderer);
        // m_scriptEngine.SetGlobalFunction("set_skybox", &Renderer::SetSkybox, m_renderer);

        // lua.set_function("clear_color_v", [&](const Math::Vec4& color) { renderer->ClearColor(color); });

        // lua.set_function("clear_color", [&](const float r, const float g, const float b, const float a) {
        //     renderer->ClearColor(r, g, b, a);
        //     renderer->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // });
    }

    std::optional<GameConfig> Application::LoadConfig()
    {
        VK_INFO("Loading game_config.lua...");

        LuaScript configScript = m_scriptEngine.CreateScript("scripts/game_config.lua");

        sol::table config = m_scriptEngine.GetScriptVariable(configScript, "game_config");

        sol::optional<std::string> name = config["name"];
        if (!name)
        {
            VK_ERROR("CONFIG ERROR: Game Name Not Set");
            return std::nullopt;
        }
        sol::optional<int> window_width = config["window"]["w"];
        if (!window_width)
        {
            VK_ERROR("CONFIG ERROR: Window Width Not set");
            return std::nullopt;
        }
        sol::optional<int> window_height = config["window"]["h"];
        if (!window_height)
        {
            VK_ERROR("CONFIG ERROR: Window Height Not Set");
            return std::nullopt;
        }
        sol::optional<std::string> renderer_type = config["renderer"];
        if (!renderer_type)
        {
            VK_ERROR("CONFIG ERROR: Renderer Not Set");
            return std::nullopt;
        }

        if (sol::optional<std::string> model_dir = config["model_dir"]; !model_dir)
        {
            VK_WARN("CONFIG WARNING: No Model Directory Set, Using Default {0}", AssetLoader::model_path);
        }
        else
        {
            AssetLoader::model_path = model_dir.value();
        }

        if (sol::optional<std::string> texture_dir = config["texture_dir"]; !texture_dir)
        {
            VK_WARN("CONFIG WARNING: No Texture Directory Set, Using Default {0}", AssetLoader::texture_path);
        }
        else
        {
            AssetLoader::texture_path = texture_dir.value();
        }

        if (sol::optional<std::string> shader_dir = config["shader_dir"]; !shader_dir)
        {
            VK_WARN("CONFIG WARNING: No Shader Directory Set, Using Default {0}", AssetLoader::shader_path);
        }
        else
        {
            AssetLoader::shader_path = shader_dir.value();
        }

        GameConfig cfg = {name.value(), window_width.value(), window_height.value(), renderer_type.value()};

        return cfg;
    }

    void Application::Run()
    {
        double physicsAccumulator = 0.0;

        bool set = false;

        while (m_running)
        {
            m_time.Update();
            m_time.accumulator += m_time.deltaTime;

            m_gui.CreateNewFrame();

            if (IsSystemActive(SystemFlag::Rendering))
            {

                Rendering::RenderEngine::PreDraw();
            }

            m_sceneManager.Update();

            Scene& activeScene = m_sceneManager.GetActiveScene();
            if (activeScene.getName() == "sandbox" && !set)
            {
                // BALL
                {

                    Entity ent = activeScene.CreateEntity("Ball");

                    ent.AddComponent<Rendering::Drawable>();

                    auto& model = AssetLoader::GetModel("coreAssets/models/sphere.obj", 1);
                    auto& drawable = ent.GetComponent<Rendering::Drawable>();

                    Rendering::RenderEngine::GenerateModel(model, drawable);

                    auto& trans = ent.GetComponent<Components::Transform>();
                    trans.pos = Math::Vec3(0.0f, 30.0f, 0.0f);

                    RigidBody rb = activeScene.GetPhysicsScene().CreateRigidBody(trans.pos, trans.rot);

                    SphereCollider collider = m_physicsEngine.CreateSphereCollider(1.0);
                    m_physicsEngine.AttachCollider(rb, collider);

                    ent.AddComponent<RigidBody>(rb);
                    ent.AddComponent<SphereCollider>(collider);
                }

                // BOX
                {
                    Entity ent = activeScene.CreateEntity("Box");

                    ent.AddComponent<Rendering::Drawable>();

                    auto& model = AssetLoader::GetModel("coreAssets/models/cube.obj", 1);
                    auto& drawable = ent.GetComponent<Rendering::Drawable>();

                    Rendering::RenderEngine::GenerateModel(model, drawable);

                    auto& trans = ent.GetComponent<Components::Transform>();
                    trans.pos = Math::Vec3(0.0f, 2.0f, 0.0f);

                    RigidBody rb = activeScene.GetPhysicsScene().CreateRigidBody(trans.pos, trans.rot);
                    rb.type = BodyType::Static;

                    Math::Vec3 halfExts = trans.scale;
                    AABBCollider collider = m_physicsEngine.CreateAABBCollider(halfExts);
                    m_physicsEngine.AttachCollider(rb, collider);

                    ent.AddComponent<RigidBody>(rb);
                    ent.AddComponent<AABBCollider>(collider);
                }

                // Floor
                // {
                //     Entity ent = activeScene.CreateEntity("Floor");

                //     ent.AddComponent<Rendering::Drawable>();

                //     auto& model = AssetLoader::GetModel("coreAssets/models/cube.obj", 1);
                //     auto& drawable = ent.GetComponent<Rendering::Drawable>();

                //     Rendering::RenderEngine::GenerateModel(model, drawable);

                //     auto& trans = ent.GetComponent<Components::Transform>();
                //     trans.pos = Math::Vec3(0.0f, 2.0f, 0.0f);
                //     trans.scale = Math::Vec3(100.0f, 1.0f, 100.0f);

                //     RigidBody rb = activeScene.GetPhysicsScene().CreateRigidBody(trans.pos, trans.rot);
                //     rb.type = BodyType::Static;

                //     Math::Vec3 halfExts = trans.scale * 0.5f;
                //     AABBCollider collider = m_physicsEngine.CreateAABBCollider(halfExts);
                //     m_physicsEngine.AttachCollider(rb, collider);

                //     ent.AddComponent<RigidBody>(rb);
                //     ent.AddComponent<AABBCollider>(collider);
                // }

                set = true;
            }

            if (IsSystemActive(SystemFlag::Physics))
            {

                // Add the time difference in the accumulator
                physicsAccumulator += m_time.deltaTime;

                // While there is enough accumulated time take one or several physics steps
                while (physicsAccumulator >= m_physicsEngine.GetTimeStep())
                {
                    // apply forces
                    activeScene.GetEntityList().Iterate<Components::Transform, RigidBody>(
                        [&](Components::Transform& transform, RigidBody& rb) {
                            m_physicsEngine.ApplyForces(transform.pos, transform.rot, rb);
                        });

                    // detect collisions
                    m_physicsEngine.DetectCollisions(activeScene.GetPhysicsScene());

                    // resolve collisions
                    activeScene.GetEntityList().Iterate<Components::Transform, RigidBody>(
                        [&](Components::Transform& transform, RigidBody& rb) {
                            if (rb.type == BodyType::Static)
                                return;
                            m_physicsEngine.ResolveCollisions(transform.pos, transform.rot, rb);
                        });

                    // Decrease the accumulated time
                    physicsAccumulator -= m_physicsEngine.GetTimeStep();
                }
            }

            while (m_time.accumulator >= m_time.tickRate)
            {
                if (IsSystemActive(SystemFlag::Scripting))
                {
                    activeScene.GetEntityList().Iterate<LuaScript>(
                        [&](auto& script) { m_scriptEngine.TickScript(script); });

                    m_scriptEngine.TickScript(activeScene.GetScript());
                }

                m_layerManager.OnTick();

                // Decrease the accumulated time
                m_time.accumulator -= m_time.tickRate;
            }

            // Compute the time interpolation factor
            // float alpha = m_time.accumulator / m_time.tickRate;

            if (IsSystemActive(SystemFlag::Scripting))
            {
                activeScene.GetEntityList().Iterate<LuaScript>(
                    [&](auto& script) { m_scriptEngine.UpdateScript(script); });

                m_scriptEngine.UpdateScript(activeScene.GetScript());
            }

            if (IsSystemActive(SystemFlag::Rendering))
            {
                activeScene.GetEntityList().Iterate<Components::Transform, Rendering::Drawable>(
                    [&](Components::Transform& transform, const Rendering::Drawable& drawable) {
                        if (drawable.active)
                            Rendering::RenderEngine::Draw(activeScene.GetCamera(), transform, drawable);
                    });

                if (activeScene.GetSkybox().active)
                {
                    Rendering::RenderEngine::DrawSkybox(activeScene.GetCamera(), activeScene.GetSkybox());
                }
            }

            activeScene.GetCamera().Update();

            if (IsSystemActive(SystemFlag::Rendering))
                Rendering::RenderEngine::PostDraw();

            m_layerManager.OnUpdate();
            m_gui.Update();

            m_input.Update();
            m_window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& ev)
    {
        EventDispatcher dispatcher(ev);

        m_layerManager.OnEvent(ev); // go through layers before hitting application.

        if (ev.Handled)
            return;

        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMoved));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleased));
    }

    void Application::SetActiveMouse(const bool active)
    {
        if (active)
            glfwSetInputMode(m_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(m_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    GameState Application::GetGameState() const
    {
        return m_gameState;
    }

    void Application::SetGameState(GameState state)
    {
        m_gameState = state;

        if (m_gameState == GameState::Paused)
        {
            // Deactivate script execution and physics simulation on pause
            m_activeSystems &= ~(static_cast<int>(SystemFlag::Scripting) | static_cast<int>(SystemFlag::Physics));
        }
        else if (m_gameState == GameState::Running)
        {
            // Reactivate script execution and physics simulation when running
            m_activeSystems |= static_cast<int>(SystemFlag::Scripting) | static_cast<int>(SystemFlag::Physics);
        }
    }

    void Application::ToggleSystem(SystemFlag system)
    {
        // Toggle the bit representing the system using the mask
        m_activeSystems ^= static_cast<int>(system);
    }

    bool Application::IsSystemActive(SystemFlag system) const
    {
        int flagValue = static_cast<int>(system);
        return (m_activeSystems & flagValue) != 0;
    }

    bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& ev)
    {
        m_running = false;

        return true;
    }

    bool Application::OnWindowResize(const WindowResizeEvent& ev) const
    {
        glViewport(0, 0, ev.GetWidth(), ev.GetHeight());
        return true;
    }

    bool Application::OnKeyPressed(KeyPressedEvent& kev)
    {
        m_input.OnKeyPressed(kev);

        return true;
    }

    bool Application::OnKeyReleased(KeyReleasedEvent& kev)
    {
        m_input.OnKeyReleased(kev);

        return true;
    }

    bool Application::OnMouseMoved(MouseMovedEvent& ev)
    {
        m_input.OnMouseMoved(ev);

        return true;
    }

    bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& mev)
    {
        m_input.OnMouseButtonPressed(mev);

        return true;
    }

    bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& mev)
    {
        m_input.OnMouseButtonReleased(mev);

        return true;
    }

    bool Application::IsRunning() const
    {
        return m_running;
    }

    void Application::PushLayer(std::shared_ptr<Layer> layer)
    {

        if (layer)
        {
            layer->OnAttach(&m_sceneManager);
            m_layerManager.PushLayer(layer);
        }
    }

    void Application::PopLayer()
    {

        m_layerManager.PopLayer();
    }

    const std::shared_ptr<Window> Application::GetWindow() const
    {
        return m_window;
    }

} // namespace Vakol