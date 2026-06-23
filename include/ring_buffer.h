#pragma once
#include <atomic>

namespace SPSC {
    template <typename T, std::size_t N>
    // I should probably be able to scale N
    class RingBuffer {
    public:
        RingBuffer() = default;
        bool push(T elem);
        bool pop(T& out);
        void finish() noexcept { done_.store(true, std::memory_order_release); }
        bool closed() const noexcept { return done_.load(std::memory_order_acquire); }
    private:
        alignas(64) std::atomic_size_t front_{0};
        alignas(64) std::size_t cached_back_{0};
        alignas(64) std::atomic_size_t back_{0};
        alignas(64) std::size_t cached_front_{0};
        alignas(64) T data_[N];
        std::atomic<bool> done_{false};
    };

    template <typename T, std::size_t N>
    bool RingBuffer<T, N>::push(T elem) {
        std::size_t back, front;

        back = std::atomic_load_explicit(&back_, std::memory_order_relaxed);
        front = cached_front_;
        if ((back + 1) % N == front) {
            cached_front_ = front = std::atomic_load_explicit(&front_, std::memory_order_acquire);
        }
        
        if ((back + 1) % N == front) {
            return false;
        }

        data_[back] = elem;
        // give back_, store the size remaining, enforce memory sequencing
        std::atomic_store_explicit(&back_, (back + 1) % N, std::memory_order_release);
        return true;
    }

    template <typename T, std::size_t N>
    bool RingBuffer<T, N>::pop(T& out) {
        std::size_t back, front;
        front = std::atomic_load_explicit(&front_, std::memory_order_relaxed);
        back = cached_back_;
        if (back == front) {
           cached_back_ = back = std::atomic_load_explicit(&back_, std::memory_order_acquire); 
        }

        if (back == front) {
            return false;
        }

        out = data_[front];
        std::atomic_store_explicit(&front_, (front + 1) % N, std::memory_order_release);
        return true;
    }

}
