#include "LuaAccess.hpp"
#include "Time/Time.hpp"

namespace Vakol
{
    void RegisterTime(sol::state& lua)
    {
        auto time_type = lua.new_usertype<Time>("Time");
        time_type["delta_time"] = &Time::deltaTime;
        time_type["tick_rate"] = &Time::tickRate;
        time_type["curr_time"] = &Time::curTime;
        time_type["prev_time"] = &Time::prevTime;
        time_type["fps"] = &Time::fps;
    }
} // namespace Vakol