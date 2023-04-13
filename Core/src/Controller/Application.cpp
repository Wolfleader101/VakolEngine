#include "Application.hpp"

#include <Controller/LuaAccess.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Physics/ScenePhysics.hpp>
#include <Model/Components.hpp>
#include <View/Renderer/RendererFactory.hpp>

#include "Logger.hpp"

#include <Model/Components.hpp>

//testing assetLoader
#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Model/Assets/Texture.hpp>

#include <Controller/System.hpp>

#include <Controller/Scene.hpp>

namespace Vakol::Controller {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() : m_running(false), m_window(nullptr), m_renderer(nullptr) 
    { 
        Logger::Init();
    };

    void Application::Init() {
        Controller::RegisterLogger(lua.GetState());
        Controller::RegisterApplication(lua.GetState(), this);
        Controller::RegisterScene(lua.GetState());

        auto config = LoadConfig();
        if (!config) {
            VK_CRITICAL("CONFIG COULD NOT BE LOADED");
            return;
        }

        m_window = std::make_shared<View::Window>(config.value().name, config.value().windowWidth,
                                                  config.value().windowHeight);

        m_renderer = CreateRenderer(config.value().rendererType, m_window);

        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        // Physics::Debug = false;

        // Physics::Init();

        VK_INFO("Calling main.lua...");

        lua.RunFile("scripts/main.lua");

        sol::function luaMain = lua.GetState()["main"];

        luaMain();

        auto x = AssetLoader::GetTexture("coreAssets/textures/pisikek.png");
        auto y = AssetLoader::GetModel("coreAssets/models/cube.obj");
        auto z = AssetLoader::GetShader("coreAssets/shaders/basic.prog");

        //physics test 

        Scene test("Physics Test Scene", "testScene.lua", lua, std::make_shared<Physics::ScenePhysics>(PhysicsPool::CreatePhysicsWorld()), true);

        for (int i = 0; i < 10; i++)
        {
            auto entity = test.entityList.CreateEntity();
            entity.GetComponent<Components::Tag>().tag = "Entity: " + std::to_string(i);
            entity.AddComponent<Components::PhysicsObject>(test.ScenePhysics, 2, 3);

            entity.AddComponent<Components::Drawable>();
            auto& draw = (entity.GetComponent<Components::Drawable>());
            draw.ModelPtr = y;



            auto& trans = entity.GetComponent<Components::Transform>();

            auto& PhyObj = entity.GetComponent<Components::PhysicsObject>();
            System::Physics_InitObject(test.ScenePhysics, PhyObj, draw, trans);
        }

        test.Serialize("assets/scenes");

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

        Model::GameConfig cfg = {name.value(), windowWidth.value(), windowHeight.value(), rendererType.value()};

        return cfg;
    }

    Application::~Application() {
        // json::SerializeScene("assets/json/Deserialize.json", m_entityList);
    }

    void Application::Run() {
        while (m_running) {
            m_time.Update();

            // Physics::FixedUpdate(m_time, m_entityList.GetEntityList());

            //! update scenes lua
            //! only update if the scene is active
            for (auto& scene : scenes) {
                if (scene.active) scene.Update(m_time);
            }

            m_renderer->Update(m_time);

            m_window->OnUpdate();
        }

        // ImGui_ImplOpenGL3_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();
    }

    void Application::AddScene(std::string scriptName, std::string scene_name) {

        std::string sceneName = scene_name.length() == 0 ? "Scene" + std::to_string(scenes.size()) : scene_name;

        scenes.push_back(Scene(sceneName, scriptName, 
                                lua,
                                std::make_shared<Physics::ScenePhysics>(Physics::PhysicsPool::CreatePhysicsWorld()),
                                true
                            ));
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