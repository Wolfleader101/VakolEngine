#include "LuaAccess.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "GUI/GUIWindow.hpp"

namespace Vakol
{
    void RegisterGUIWindow(sol::state& lua)
    {
        auto gui_window_type = lua.new_usertype<GUIWindow>("GUI");

        gui_window_type.set_function("get_display_window_width", &GUIWindow::DisplayWindowWidth);
        gui_window_type.set_function("get_display_window_height", &GUIWindow::DisplayWindowHeight);

        gui_window_type.set_function("start_window", &GUIWindow::StartWindowCreation);

        gui_window_type.set_function("update", &GUIWindow::Update);

        gui_window_type.set_function("start_frame", &GUIWindow::CreateNewFrame);
        gui_window_type.set_function("end_frame", &GUIWindow::EndFrame);

        gui_window_type.set_function("get_fps", &GUIWindow::GetFramesPerSecond);

        gui_window_type.set_function("add_text", &GUIWindow::AddText);
        gui_window_type.set_function("add_button", &GUIWindow::AddButton);
        gui_window_type.set_function("add_checkbox", &GUIWindow::AddCheckbox);

        gui_window_type.set_function("add_image", [](const GUIWindow* GUI, const std::string& path, const float width,
                                                     const float height, const bool centerX, const bool centerY) {
            const auto& tex = AssetLoader::GetTexture(path, false, false);
            const unsigned int texID = tex->GetID();

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