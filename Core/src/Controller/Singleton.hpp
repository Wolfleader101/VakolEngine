#pragma once

namespace Vakol::Controller {
template <class T>
class Singleton {
   public:
    Singleton(Singleton& other) = delete;

    void operator=(const Singleton&) = delete;

    static T& GetInstance() {
        static T instance;
        return instance;
    }

   protected:
    Singleton() = default;
};
}  // namespace Vakol::Controller