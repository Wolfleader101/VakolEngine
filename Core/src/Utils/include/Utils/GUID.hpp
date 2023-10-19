#pragma once

#include <string>

#include <crossguid/guid.hpp>

namespace Vakol
{
    /**
     * @brief A class for generating GUIDs
     *
     */
    class GUID
    {
      public:
        /**
         * @brief Default constructor for the GUID class.
         */
        GUID();

        /**
         * @brief Function to generate a new GUID.
         */
        void GenNewGUID();

        /**
         * @brief Function to convert the GUID to a string.
         */
        std::string ToString() const;

        /**
         * \brief determine if the guid id is valid or not
         * \return the validity of the ID
         */
        bool IsValid() const;

        /**
         * @brief Equality operator for GUID.
         * @param other The other GUID to compare with.
         * @return True if the GUIDs are equal, false otherwise.
         */
        bool operator==(const GUID& other) const;

        /**
         * @brief Inequality operator for GUID.
         * @param other The other GUID to compare with.
         * @return True if the GUIDs are not equal, false otherwise.
         */
        bool operator!=(const GUID& other) const;

        /**
         * @brief Less than operator for GUID.
         * @param other The other GUID to compare with.
         * @return True if the current GUID is less than the other GUID, false otherwise.
         */
        bool operator<(const GUID& other) const;

        /**
         * @brief Function to save the GUID.
         * @param ar Archive to save the GUID to.
         */
        template <class Archive>
        void save(Archive& ar) const
        {
            std::string id_str = id.str();
            ar(cereal::make_nvp("guid", id_str));
        }

        /**
         * @brief Function to load the GUID.
         * @param ar Archive to load the GUID from.
         */
        template <class Archive>
        void load(Archive& ar)
        {
            std::string id_str;
            ar(cereal::make_nvp("guid", id_str));
            id = xg::Guid(id_str);
        }

      private:
        xg::Guid id; ///< The actual GUID.
    };
} // namespace Vakol