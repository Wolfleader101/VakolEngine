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

TEST_CASE("LayerManager Functionality", "[LayerManager]")
{
    Vakol::Application app = Vakol::Application();

    Vakol::LayerManager layerManager;

    SECTION("Push and Pop Layer")
    {
        std::shared_ptr<TestLayer> layer = std::make_shared<TestLayer>(app);
        layerManager.PushLayer(layer);
        REQUIRE(layer->attached == true); // confirm OnAttach was called

        layerManager.PopLayer();
        REQUIRE(layer->attached == false); // confirm OnDetach was called
    }

    SECTION("Update Layers")
    {
        auto layer = std::make_shared<TestLayer>(app);
        layerManager.PushLayer(layer);
        layerManager.OnUpdate();

        REQUIRE(layer->updated == true); // confirm OnUpdate was called
    }

    SECTION("Event Handling")
    {
        auto layer = std::make_shared<TestLayer>(app);
        layerManager.PushLayer(layer);
        Vakol::TestEvent event;
        layerManager.OnEvent(event);

        REQUIRE(layer->eventReceived == true); // confirm OnEvent was called
        REQUIRE(event.Handled == true);        // confirm event was handled
    }

    SECTION("Tick Handling")
    {
        auto layer = std::make_shared<TestLayer>(app);
        layerManager.PushLayer(layer);
        layerManager.OnTick();

        REQUIRE(layer->ticked == true); // confirm OnTick was called
    }
}