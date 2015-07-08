#include <future>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

struct nameWorker {
    bool m_status = false;
    std::future<std::string> m_future;
    void doWork(std::string _name) {
        m_future = std::async(std::launch::async, [&](std::string n) { // &: capture any referenced variables by reference
                if(n == "name3") {
                    std::this_thread::sleep_for(std::chrono::microseconds(1000));
                }
                m_status = true;
                return n;
        }, _name);
    }
};

int main() {
    std::vector<std::string> names{"name1", "name2", "name3", "name4", "name5"};

    std::vector<nameWorker*> workers;

    for(auto& name : names) {
        nameWorker* worker = new nameWorker();
        worker->doWork(name);
        workers.push_back(worker);
    }

    while(true) {
        auto it = workers.begin();
        while(it != workers.end()) {
            if((*it)->m_status) {
                std::cout<<(*it)->m_future.get()<<std::endl;
                it = workers.erase(it);
            } else { it++; }
        }
        if(workers.size() == 0) { break; }
    }

    return 0;
}

