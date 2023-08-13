#include "Serialisation/SolSerialize.hpp"

namespace Vakol
{

    // std::function<void(const sol::table&, SolTableData&)> recursiveSolToMap = [&](const sol::table& table,
    //                                                                               SolTableData& solTableData) {
    //     table.for_each([&](const sol::object& key, const sol::object& value) {
    //         if (value.get_type() == sol::type::table) {
    //             SolTableData nestedTableData;
    //             recursiveSolToMap(value.as<sol::table>(), nestedTableData);
    //             solTableData.data[key.as<std::string>()] = nestedTableData;
    //         } else {
    //             if (value.is<int>())
    //                 solTableData.data[key.as<std::string>()] = value.as<int>();

    //             else if (value.is<float>())
    //                 solTableData.data[key.as<std::string>()] = value.as<float>();

    //             else if (value.is<bool>())
    //                 solTableData.data[key.as<std::string>()] = value.as<bool>();

    //             else if (value.is<std::string>())
    //                 solTableData.data[key.as<std::string>()] = value.as<std::string>();

    //             else if (value.is<Math::Vec3>())
    //                 solTableData.data[key.as<std::string>()] = value.as<Math::Vec3>();
    //         }
    //     });
    // };

    // // Function to recursively convert sol2 table to SolTableData structure
    // void ConvertSolToMap(const sol::table& table, SolTableData& solTableData) {
    //     recursiveSolToMap(table, solTableData);
    // }

    // // Function to recursively convert SolTableData structure to sol2 table with callback functions
    // void ConvertMapToSol(std::shared_ptr<LuaState> lua, const SolTableData& solTableData, sol::table& table) {
    //     for (const auto& [key, value] : solTableData.data) {
    //         if (auto nestedSolTableData = std::get_if<SolTableData>(&value)) {
    //             sol::table nestedTable = lua->GetState().create_table();
    //             ConvertMapToSol(lua, *nestedSolTableData, nestedTable);
    //             table[key] = nestedTable;
    //         } else {
    //             table[key] = value;
    //         }
    //     }
    // }

} // namespace Vakol