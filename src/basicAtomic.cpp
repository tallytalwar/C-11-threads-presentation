#include <atomic>
#include <iostream>
#include <future>
#include <thread>
#include <vector>

struct Counter {
    std::atomic<int> value;
    //int value; // This will cause data race

    Counter() : value(0){}

    void increment(){ ++value; }

    void decrement() { --value; }

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

    std::cout << counter.value.load() << std::endl;
    //std::cout << counter.value << std::endl;
    return 0;
}
