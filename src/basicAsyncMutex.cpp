#include <iostream>
#include <mutex>
#include <future>
#include <thread>
#include <vector>

struct Counter {
    int value;
    std::mutex m_mutex;

    Counter() : value(0){}

    void increment(){
        // RAII of doing mutex lock
        std::lock_guard<std::mutex> lock(m_mutex);
        ++value;
    }

    void decrement() {
        std::lock_guard<std::mutex> lock(m_mutex);
        --value;
    }
};

int main() {
    Counter counter;

    std::vector<std::future<void>> futs;

    for(int i = 0; i < 5; ++i){
        futs.push_back( std::async(std::launch::async, [&counter](){
                        for(int i = 0; i < 10000; ++i) {
                            counter.increment();
                        }
                    }) );
    }

    for(int i = 0; i < 5; i++) {
        futs[i].get();
    }

    std::cout << counter.value << std::endl;
    return 0;
}
