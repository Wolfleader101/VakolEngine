#pragma once

#include <queue>

namespace Vakol::Rendering
{
    template <class T>
    class RenderQueue
    {
    public:
        void Emplace(T& item);
        void Push(T& item);

        void Pop();

        const T& Front() const;
        const T& Back() const;

    private:
        std::vector<T> m_queue;
    };

    template <class T>
    void RenderQueue<T>::Push(T& item)
    {
        m_queue.push_backs(item);
    }

    template <class T>
    void RenderQueue<T>::Emplace(T& item)
    {
        m_queue.emplace_back(item);
    }

    template <class T>
    void RenderQueue<T>::Pop()
    {
        m_queue.pop_back();
    }

    template <class T>
    const T& RenderQueue<T>::Front() const
    {
        return m_queue.front();   
    }

    template <class T>
    const T& RenderQueue<T>::Back() const
    {
        return m_queue.back();
    }

}

