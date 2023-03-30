#include "Application.hpp"

#include <Controller/LuaAccess.hpp>
#include <View/Renderer/RendererFactory.hpp>

#include "Logger.hpp"

#include <Model/Components.hpp>
// #include "JSON/Json.hpp"
// #include "Physics/Physics.hpp"

namespace Vakol::Controller {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    bool testBoolean = false;
    int testInteger = 2;
    int testIntegerArray[] = {2, 3};
    int testIntegerArrayTwo[] = {3, 1, 4};
    int testIntegerArrayThree[] = {3, 1, 4, 2};
    float testFloat = 0.0f;
    float testFloatArray[] = {2.0f, 3.0f};
    float testFloatArrayTwo[] = {3.0f, 1.0f, 4.0f};
    float testFloatArrayThree[] = {3.0f, 1.0f, 4.0f, 2.0f};

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

        Model::GameConfig cfg = {name.value(), windowWidth.value(), windowHeight.value(), rendererType.value()};

        return cfg;
    }

    Application::~Application() {
        // json::SerializeScene("assets/json/Deserialize.json", m_entityList);
    }

    void PrintStuff() { std::cout << "This is a test!" << std::endl; }

    void Application::Run() {
        while (m_running) {
            m_time.Update();

            // Physics::FixedUpdate(m_time, m_entityList.GetEntityList());

            //! update scenes lua
            for (auto& scene : scenes) {
                scene.Update(m_time);
            }

            m_renderer->Update(m_time);

            // m_gui.OnUpdate();

            m_gui.CreateNewFrame();

            m_gui.StartWindowCreation(std::string("A Very New Window"), 640, 480, 0, 0);
            m_gui.AddButton(std::string("Cool Button"), 100, 20, PrintStuff);
            m_gui.AddCheckbox(std::string("Cool Checkbox"), testBoolean);
            m_gui.AddIntSlider(std::string("Cool Slider Int"), testInteger, -10, 10);
            m_gui.AddVecIntSlider(std::string("Cool Slider Vec 2 Int"), testIntegerArray, 2, -2, 5);
            m_gui.AddVecIntSlider(std::string("Cool Slider Vec 3 Int"), testIntegerArrayTwo, 3, -2, 5);
            m_gui.AddVecIntSlider(std::string("Cool Slider Vec 4 Int"), testIntegerArrayThree, 4, -2, 5);
            m_gui.AddFloatSlider(std::string("Cool Slider Float"), testFloat, -10.0, 10.0);
            m_gui.AddVecFloatSlider(std::string("Cool Slider Vec 2 Float"), testFloatArray, 2, -2.0f, 5.0f);
            m_gui.AddVecFloatSlider(std::string("Cool Slider Vec 3 Float"), testFloatArrayTwo, 3, -2.0f, 5.0f);
            m_gui.AddVecFloatSlider(std::string("Cool Slider Vec 4 Float"), testFloatArrayThree, 4, -2.0f, 5.0f);
            m_gui.EndWindowCreation();

            if (testBoolean || testInteger == 10) {
                PrintStuff();
            }

            m_window->OnUpdate();
        }

        // ImGui_ImplOpenGL3_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();
    }

    void Application::AddScene(std::string scriptName, std::string scene_name) {
        std::string sceneName = scene_name.length() == 0 ? "Scene" + std::to_string(scenes.size()) : scene_name;
        scenes.push_back(Scene(sceneName, scriptName, lua));
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