#include "Input/Input.hpp"
#include "LuaAccess.hpp"

namespace Vakol
{
    void RegisterInput(sol::state& lua)
    {
        auto input_type = lua.new_usertype<Input>("Input");
        input_type.set_function("get_key", &Input::GetKey);
        input_type.set_function("get_key_down", &Input::GetKeyDown);
        input_type.set_function("get_key_up", &Input::GetKeyUp);
        input_type.set_function("get_mouse", &Input::GetMouseButton);
        input_type.set_function("get_mouse_down", &Input::GetMouseButtonDown);
        input_type.set_function("get_mouse_up", &Input::GetMouseButtonUp);
        input_type.set_function("get_mouse_pos", &Input::GetMousePos);
        input_type.set_function("get_delta_mouse_pos", &Input::GetDeltaMousePos);

        lua["KEYS"] = lua.create_table_with(
            "MOUSE_0", Input::KEY::KEY_MOUSE_0, "MOUSE_1", Input::KEY::KEY_MOUSE_1, "KEY_SPACE", Input::KEY::KEY_SPACE,
            "KEY_APOSTROPHE", Input::KEY::KEY_APOSTROPHE, "KEY_COMMA", Input::KEY::KEY_COMMA, "KEY_MINUS",
            Input::KEY::KEY_MINUS, "KEY_PERIOD", Input::KEY::KEY_PERIOD, "KEY_SLASH", Input::KEY::KEY_SLASH, "KEY_0",
            Input::KEY::KEY_0, "KEY_1", Input::KEY::KEY_1, "KEY_2", Input::KEY::KEY_2, "KEY_3", Input::KEY::KEY_3,
            "KEY_4", Input::KEY::KEY_4, "KEY_5", Input::KEY::KEY_5, "KEY_6", Input::KEY::KEY_6, "KEY_7",
            Input::KEY::KEY_7, "KEY_8", Input::KEY::KEY_8, "KEY_9", Input::KEY::KEY_9, "KEY_SEMICOLON",
            Input::KEY::KEY_SEMICOLON, "KEY_EQUAL", Input::KEY::KEY_EQUAL, "KEY_A", Input::KEY::KEY_A, "KEY_B",
            Input::KEY::KEY_B, "KEY_C", Input::KEY::KEY_C, "KEY_D", Input::KEY::KEY_D, "KEY_E", Input::KEY::KEY_E,
            "KEY_F", Input::KEY::KEY_F, "KEY_G", Input::KEY::KEY_G, "KEY_H", Input::KEY::KEY_H, "KEY_I",
            Input::KEY::KEY_I, "KEY_J", Input::KEY::KEY_J, "KEY_K", Input::KEY::KEY_K, "KEY_L", Input::KEY::KEY_L,
            "KEY_M", Input::KEY::KEY_M, "KEY_N", Input::KEY::KEY_N, "KEY_O", Input::KEY::KEY_O, "KEY_P",
            Input::KEY::KEY_P, "KEY_Q", Input::KEY::KEY_Q, "KEY_R", Input::KEY::KEY_R, "KEY_S", Input::KEY::KEY_S,
            "KEY_T", Input::KEY::KEY_T, "KEY_U", Input::KEY::KEY_U, "KEY_V", Input::KEY::KEY_V, "KEY_W",
            Input::KEY::KEY_W, "KEY_X", Input::KEY::KEY_X, "KEY_Y", Input::KEY::KEY_Y, "KEY_Z", Input::KEY::KEY_Z,
            "KEY_LEFT_SHIFT", Input::KEY::KEY_LEFT_SHIFT, "KEY_ESC", Input::KEY::KEY_ESCAPE);
    }

}