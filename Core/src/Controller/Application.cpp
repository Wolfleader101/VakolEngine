#include "Application.hpp"

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/LuaAccess.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Controller/System.hpp>
#include <Model/Components.hpp>
#include <View/Renderer/RendererFactory.hpp>

#include "Logger.hpp"

int numParticles = 0;
std::vector<Vakol::Physics::Particle> particles(0);

float Random(float min, float max) {
    if (max < min) {
        float t = min;
        min = max;
        max = t;
    }

    float random = ((float)rand()) / (float)RAND_MAX;

    float range = max - min;
    return (random * range) + min;
}

Vakol::Math::Vec3 Random(Vakol::Math::Vec3 min, Vakol::Math::Vec3 max) {
    Vakol::Math::Vec3 result;
    result.x = Random(min.x, max.x);
    result.y = Random(min.y, max.y);
    result.z = Random(min.z, max.z);
    return result;
}

constexpr float DEG2RAD(float degree) { return degree * 3.14159265358979323846f / 180.0f; }

Vakol::Math::Mat3 XRotation3x3(float angle) {
    angle = DEG2RAD(angle);
    return Vakol::Math::Mat3(1.0f, 0.0f, 0.0f, 0.0f, cosf(angle), sinf(angle), 0.0f, -sinf(angle), cosf(angle));
}

Vakol::Math::Mat3 YRotation3x3(float angle) {
    angle = DEG2RAD(angle);
    return Vakol::Math::Mat3(cosf(angle), 0.0f, -sinf(angle), 0.0f, 1.0f, 0.0f, sinf(angle), 0.0f, cosf(angle));
}

Vakol::Math::Mat3 ZRotation3x3(float angle) {
    angle = DEG2RAD(angle);
    return Vakol::Math::Mat3(cosf(angle), sinf(angle), 0.0f, -sinf(angle), cosf(angle), 0.0f, 0.0f, 0.0f, 1.0f);
}

Vakol::Math::Mat3 Rotation3x3(float pitch, float yaw, float roll) {
    return ZRotation3x3(roll) * XRotation3x3(pitch) * YRotation3x3(yaw);
}

namespace Vakol::Controller {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() : m_window(nullptr), m_renderer(nullptr), m_running(false), m_input() {
        Logger::Init();
        scenes.reserve(10);
        lua = std::make_shared<LuaState>();
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

        lua->RunFile("scripts/main.lua");

        const sol::function lua_main = lua->GetState()["main"];

        m_running = true;

        // TODO remove later
        numParticles = 50;
        particles.reserve(numParticles);

        m_physicsEngine.ClearRigidbodys();
        m_physicsEngine.ClearConstraints();

        Vakol::Physics::OBB ground;
        ground.size = Math::Vec3(10.0f, 0.15f, 10.0f);

        Vakol::Physics::OBB obb1;
        obb1.pos = Math::Vec3(0.0f, 1.86f, -1.92f);
        obb1.orientation = Rotation3x3(30.716f, 0.0f, 0.0f);
        obb1.size = Math::Vec3(2.0f, 0.15f, 2.0f);

        Vakol::Physics::OBB obb2;
        obb2.pos = Math::Vec3(-1.0f, 3.6f, 1.2f);
        obb2.orientation = Rotation3x3(-33.964f, -24.233f, 9.128f);
        obb2.size = Math::Vec3(2.0f, 0.15f, 2.0f);

        Vakol::Physics::OBB obb3;
        obb3.pos = Math::Vec3(0.0f, 3.93f, -2.27f);
        obb3.orientation = Rotation3x3(24.702f, 0.0f, 0.0f);
        obb3.size = Math::Vec3(2.0f, 0.15f, 0.7817011f);

        m_physicsEngine.AddConstraint(ground);
        m_physicsEngine.AddConstraint(obb1);
        m_physicsEngine.AddConstraint(obb2);
        m_physicsEngine.AddConstraint(obb3);

        Math::Vec3 spawnPos = Math::Vec3(-0.5f, 6.5f, -1.01f);
        Math::Vec3 spawnSize = Math::Vec3(3.8505f, 2, 4.034834f);
        Math::Vec3 spawnMin = spawnPos - spawnSize;
        Math::Vec3 spawnMax = spawnPos + spawnSize;

        for (int i = 0; i < numParticles; ++i) {
            particles.push_back(Vakol::Physics::Particle());
            particles[i].SetPosition(Random(spawnMin, spawnMax));
            particles[i].SetBounce(Random(0, 1));
            m_physicsEngine.AddRigidbody(&particles[i]);
        }
    }

    void Application::RegisterLua() {
        RegisterLogger(lua->GetState());
        RegisterMath(lua->GetState());
        RegisterEntity(lua, lua->GetState());
        RegisterECS(lua->GetState());
        RegisterAssetLoader(lua->GetState());
        RegisterApplication(lua->GetState(), this);
        RegisterRenderer(lua->GetState(), m_renderer);
        RegisterScene(lua->GetState());
        RegisterGUIWindow(lua->GetState(), &m_gui);  // Register GUI Window
        RegisterPhysics(lua->GetState());
        RegisterOther(lua->GetState());
    }

    std::optional<Model::GameConfig> Application::LoadConfig() {
        VK_INFO("Loading game_config.lua...");

        lua->RunFile("scripts/game_config.lua");

        sol::table config = lua->GetState()["game_config"];

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

            m_physicsEngine.Update(static_cast<float>(m_time.deltaTime));

            Vakol::Physics::FixedFunctionOrigin();

            float val[] = {0.0f, 1.0f, 0.0f, 0.0f};
            glLightfv(GL_LIGHT0, GL_POSITION, val);

            m_physicsEngine.Render();

            // //! update scenes lua
            for (auto& scene : scenes) {
                if (!scene.active) continue;

                if (!scene.initialized) scene.Init();

                System::BindScene(scene);

                m_renderer->UpdateData(scene.GetCamera());

                scene.Update(m_time, m_renderer);
            }

            m_renderer->LateUpdate();

            m_gui.Update();
            m_input.Update();
            m_window->OnUpdate();
        }
    }

    void Application::AddScene(const std::string& scriptName, const std::string& scene_name) {
        const std::string sceneName = scene_name.length() == 0 ? "Scene" + std::to_string(scenes.size()) : scene_name;

        const auto ref = std::make_shared<ScenePhysics>(PhysicsPool::CreatePhysicsWorld());

        scenes.emplace_back(sceneName, scriptName, lua, ref, true);
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