#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

std::vector<std::string> names{"nameA1", "nameB2", "nameC3", "nameD4", "nameE5"};

struct nameWorkers {
    std::mutex mutex;
    std::vector<std::thread> threads;
    void doWork(int i) {
        if(i == 3) {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }
        {
            std::lock_guard<std::mutex> lock(mutex);
            names[i] = std::string(names[i].begin(), names[i].end()-1);
            std::cout<<"Name from thread "<<threads[i].get_id()<<": "<<names[i]<<"\n";
        }
    }

    nameWorkers(int n) {
        for(int i = 0; i < n; ++i) {
            threads.emplace_back(&nameWorkers::doWork, this, i);
        }
    }
};

int main() {

    nameWorkers workers(5);
    for(auto& t : workers.threads) {
        t.join();
    }
    return 0;
}

