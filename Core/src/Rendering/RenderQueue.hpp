#pragma once

#include <queue>

namespace Vakol::Rendering
{
    template <class T>
    class RenderQueue {
       public:
        void AddToQueue(T&& item);

       private:
        std::queue<T> m_queue;
    };

    template <class T>
    void RenderQueue<T>::AddToQueue(T&& item) {
        m_queue.emplace(item);
    }
}

