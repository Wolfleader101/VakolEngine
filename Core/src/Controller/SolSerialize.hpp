#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/types/common.hpp>  // For basic types like int, float, and bool
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/variant.hpp>

#include "Controller/Math.hpp"
#include "LuaState.hpp"

namespace Vakol::Controller {

    struct SolTableData {
        std::unordered_map<std::string, std::variant<SolTableData, std::string, float, int, bool, glm::vec3>> data;

        template <class Archive>
        void serialize(Archive& archive) {
            archive(data);
        }
    };

    void ConvertSolToMap(const sol::table& table, SolTableData& solTableData);
    void ConvertMapToSol(std::shared_ptr<LuaState> lua, const SolTableData& solTableData, sol::table& table);

}  // namespace Vakol::Controller