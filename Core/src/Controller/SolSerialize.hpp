#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/types/common.hpp> // For basic types like int, float, and bool
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/variant.hpp>

#include "Math/include/Math.hpp"

namespace Vakol::Controller
{

    /**
     * @struct SolTableData
     *
     * @brief Struct representing the data stored in a Lua table.
     */
    struct SolTableData
    {
        /**
         * @brief The map storing the data in the Lua table.
         */
        std::unordered_map<std::string, std::variant<SolTableData, std::string, float, int, bool, Math::Vec3>> data;

        /**
         * @brief Serialize the SolTableData.
         *
         * @tparam Archive The archive type.
         * @param archive The archive object.
         */
        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(data);
        }
    };

    /**
     * @brief Convert a Lua table to a SolTableData.
     *
     * @param table The sol::table to convert.
     * @param solTableData The SolTableData to store the converted data.
     */
    // void ConvertSolToMap(const sol::table& table, SolTableData& solTableData);

    /**
     * @brief Convert a SolTableData to a Lua table.
     *
     * @param lua The shared pointer to the Lua state.
     * @param solTableData The SolTableData to convert.
     * @param table The sol::table to store the converted data.
     */
    // void ConvertMapToSol(std::shared_ptr<LuaState> lua, const SolTableData& solTableData, sol::table& table);

} // namespace Vakol::Controller