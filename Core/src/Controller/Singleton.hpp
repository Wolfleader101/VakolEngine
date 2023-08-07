#pragma once

namespace Vakol::Controller
{
    /**
     * @class Singleton
     *
     * @brief Template class for creating singleton instances.
     *
     * @tparam T The class type of the singleton.
     */
    template <class T>
    class Singleton
    {
      public:
        /**
         * @brief Deleted copy constructor.
         *
         * @param other The other singleton instance.
         */
        Singleton(Singleton& other) = delete;

        /**
         * @brief Deleted assignment operator.
         *
         * @param The other singleton instance.
         */
        void operator=(const Singleton&) = delete;

        /**
         * @brief Get the instance of the singleton.
         *
         * @return T& The reference to the singleton instance.
         */
        static T& GetInstance()
        {
            static T instance;
            return instance;
        }

      protected:
        /**
         * @brief Default constructor.
         */
        Singleton() = default;
    };
} // namespace Vakol::Controller