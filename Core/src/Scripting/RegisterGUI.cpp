#include "AssetLoader/AssetManager.hpp"
#include "LuaAccess.hpp"

#include "GUI/GUIWindow.hpp"

namespace Vakol
{
    void RegisterGUIWindow(sol::state& lua)
    {
        auto gui_window_type = lua.new_usertype<GUIWindow>("GUI");

        gui_window_type.set_function("get_display_window_width", &GUIWindow::DisplayWindowWidth);
        gui_window_type.set_function("get_display_window_height", &GUIWindow::DisplayWindowHeight);

        gui_window_type.set_function("get_gui_window_width", &GUIWindow::GUIWindowWidth);
        gui_window_type.set_function("get_gui_window_height", &GUIWindow::GUIWindowHeight);

        gui_window_type.set_function("start_window", [](GUIWindow& self, const std::string& windowName, bool centerX,
                                                        bool centerY, sol::optional<float> width,
                                                        sol::optional<float> height, sol::optional<float> xOffset,
                                                        sol::optional<float> yOffset) {
            self.StartWindowCreation(windowName, centerX, centerY, width.value_or(0.0f), height.value_or(0.0f),
                                     xOffset.value_or(0.0f), yOffset.value_or(0.0f));
        });

        gui_window_type.set_function("update", &GUIWindow::Update);

        gui_window_type.set_function("start_frame", &GUIWindow::CreateNewFrame);
        gui_window_type.set_function("end_frame", &GUIWindow::EndFrame);

        gui_window_type.set_function("get_fps", &GUIWindow::GetFramesPerSecond);

        gui_window_type.set_function("add_text", &GUIWindow::AddText);
        gui_window_type.set_function("add_button", &GUIWindow::AddButton);
        gui_window_type.set_function("add_checkbox", &GUIWindow::AddCheckbox);

        gui_window_type.set_function("add_image", [](const GUIWindow* GUI, const std::string& path, const float width,
                                                     const float height, const bool centerX, const bool centerY) {
            const auto texID = AssetManager::GetTexture(path, 0).ID;

            GUI->AddImage(texID, {width, height}, centerX, centerY);
        });

        gui_window_type.set_function("add_integer_slider", &GUIWindow::AddIntSlider);
        gui_window_type.set_function("add_float_slider", &GUIWindow::AddFloatSlider);

        gui_window_type.set_function("add_vector_integer_slider", &GUIWindow::AddVecIntSlider);
        gui_window_type.set_function("add_vector_float_slider", &GUIWindow::AddVecFloatSlider);

        gui_window_type.set_function("same_line", &GUIWindow::SameLine);

        gui_window_type.set_function("change_background_colour", &GUIWindow::WindowBackgroundStyle);
        gui_window_type.set_function("change_background_rounding", &GUIWindow::WindowRoundingStyle);

        gui_window_type.set_function("end_window", &GUIWindow::EndWindowCreation);
    }

} // namespace Vakol
