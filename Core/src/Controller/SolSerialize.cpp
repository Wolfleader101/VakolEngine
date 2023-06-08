#include "SolSerialize.hpp"

namespace Vakol::Controller {

    std::function<void(const sol::table&, SolTableData&)> recursiveSolToMap = [&](const sol::table& table,
                                                                                  SolTableData& solTableData) {
        table.for_each([&](const sol::object& key, const sol::object& value) {
            if (value.get_type() == sol::type::table) recursiveSolToMap(value.as<sol::table>(), solTableData);

            if (value.is<int>()) solTableData.data[key.as<std::string>()] = value.as<int>();

            if (value.is<float>()) solTableData.data[key.as<std::string>()] = value.as<float>();

            if (value.is<bool>()) solTableData.data[key.as<std::string>()] = value.as<bool>();

            if (value.is<std::string>()) solTableData.data[key.as<std::string>()] = value.as<std::string>();

            if (value.is<glm::vec3>()) solTableData.data[key.as<std::string>()] = value.as<glm::vec3>();
        });
    };

    // Function to recursively convert sol2 table to SolTableData structure
    void ConvertSolToMap(const sol::table& table, SolTableData& solTableData) {
        recursiveSolToMap(table, solTableData);
    }

    // Function to recursively convert SolTableData structure to sol2 table with callback functions
    void ConvertMapToSol(const SolTableData& solTableData, sol::table& table) {
        for (const auto& [key, value] : solTableData.data) {
            table[key] = value;
        }
    }
}  // namespace Vakol::Controller