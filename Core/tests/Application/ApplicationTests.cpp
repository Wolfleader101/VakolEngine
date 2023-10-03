#include <catch2/catch_all.hpp>

#include "Application/Application.hpp"
#include "LayerManager/LayerManager.hpp"
#include "Utils/Layer.hpp"
#include "Window/Events/Event.hpp"
#include <memory>

namespace Vakol
{
    class TestEvent : public Event
    {
      public:
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(Vakol::EventCategoryApplication)
    };
} // namespace Vakol

class TestLayer : public Vakol::Layer
{
  public:
    TestLayer(Vakol::Application& app) : Vakol::Layer(app)
    {
    }

    void OnAttach() override
    {
        attached = true;
    }

    void OnDetach() override
    {
        attached = false;
    }

    void OnUpdate() override
    {
        updated = true;
    }

    void OnEvent(Vakol::Event& event) override
    {
        event.Handled = true;
        eventReceived = true;
    }

    void OnTick() override
    {
        ticked = true;
    }

    bool attached = false;
    bool updated = false;
    bool eventReceived = false;
    bool ticked = false;
};

TEST_CASE("Application Construction and Basic Operations", "[Application]")
{
    Vakol::Application app("testScripts/game_config.lua");

    SECTION("Default properties after construction")
    {
        REQUIRE(app.GetWidth() == 1);
        REQUIRE(app.GetHeight() == 1);
    }

    SECTION("Setting running state")
    {
        app.SetRunning(false);
        REQUIRE(app.IsRunning() == false);

        app.SetRunning(true);
        REQUIRE(app.IsRunning() == true);
    }

    SECTION("Setting and getting game state")
    {
        app.SetGameState(Vakol::GameState::Paused);
        REQUIRE(app.GetGameState() == Vakol::GameState::Paused);

        app.SetGameState(Vakol::GameState::Running);
        REQUIRE(app.GetGameState() == Vakol::GameState::Running);
    }

    SECTION("Toggling and checking system status")
    {
        // Assuming that systems are off by default for this test.
        REQUIRE(app.IsSystemActive(Vakol::SystemFlag::Rendering) == true);
        app.ToggleSystem(Vakol::SystemFlag::Rendering);
        REQUIRE(app.IsSystemActive(Vakol::SystemFlag::Rendering) == false);

        // Toggling again should turn it off.
        app.ToggleSystem(Vakol::SystemFlag::Rendering);
        REQUIRE(app.IsSystemActive(Vakol::SystemFlag::Rendering) == true);
    }

    SECTION("Manipulating Layers")
    {
        auto layer1 = std::make_shared<TestLayer>(app);
        auto layer2 = std::make_shared<TestLayer>(app);

        app.PushLayer(layer1);

        REQUIRE(layer1->attached == true);

        app.PushLayer(layer2);

        REQUIRE(layer2->attached == true);

        app.PopLayer();

        REQUIRE(layer2->attached == false);
        REQUIRE(layer1->attached == true);

        app.PopLayer();
        REQUIRE(layer1->attached == false);

        app.PopLayer();
    }
}

TEST_CASE("Scene Handling", "[Application]")
{
    Vakol::Application app("testScripts/game_config.lua");
    app.GetSceneManager().CreateScene("testScene", "");

    SECTION("Create Test Scene")
    {
        Vakol::Scene& activeScene = app.GetSceneManager().GetActiveScene();

        REQUIRE(activeScene.getName() == "testScene");
    }

    SECTION("Changing active scene")
    {
        app.GetSceneManager().CreateScene("testScene2", "");

        app.GetSceneManager().ChangeActiveScene("testScene2");

        app.GetSceneManager().Update();

        REQUIRE(app.GetSceneManager().GetActiveScene().getName() == "testScene2");
    }
}

TEST_CASE("Event Handling", "[Application]")
{
    Vakol::Application app("testScripts/game_config.lua");

    SECTION("Window close event")
    {
        Vakol::WindowCloseEvent closeEvent;
        app.OnEvent(closeEvent);

        REQUIRE(app.IsRunning() == false);
    }
}