#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <string>
#include <list>

class reentrant_lock {
    std::condition_variable cv_;
    std::mutex m_;
    std::atomic<std::thread::id> owner_thread;
    int lock_times = 0;
    std::atomic_int thread_count{0};

public :

    bool try_lock() {
        if (owner_thread == std::this_thread::get_id()) {
            ++lock_times;
            return true;
        }
        std::thread::id null_id;
        if (owner_thread.compare_exchange_strong(null_id, std::this_thread::get_id())) {
            ++lock_times;
            return true;

        }
        return false;
    }

    void lock() {
        if (owner_thread == std::this_thread::get_id()) {
            ++lock_times;
            return;
        }
        while (true) {
            std::thread::id null_id;
            if (owner_thread.compare_exchange_strong(null_id, std::this_thread::get_id())) {
                break;

            }
            ++thread_count;
            std::unique_lock<decltype(m_)> l(m_);
            cv_.wait(l, [this] { return owner_thread == std::thread::id(); });
            --thread_count;

        }
        ++lock_times;


    }

    void unlock() {
        assert(owner_thread == std::this_thread::get_id());
        assert(lock_times > 0);
        --lock_times;
        if (lock_times == 0) {
            owner_thread = std::thread::id();
            if (thread_count > 0) {

                cv_.notify_one();

            }

        }
    }
};


int main() {

    reentrant_lock l;
    l.lock();
    l.lock();

    std::thread t1([&l] {
        l.lock();
        std::cout << "thread 1 \n";
        l.unlock();

    });

    std::thread t2([&l] {
        if (l.try_lock()) {
            std::cout << "thread 2 \n";
            l.unlock();
        }
    });

    l.unlock();
    l.unlock();

    t1.join();
    t2.join();
}
