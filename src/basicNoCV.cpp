#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <future>

struct worker {
    std::string data;
    std::thread thread;
    std::future<void> signalFromMain;
    std::promise<void> signalMain;

    void worker_thread() {
        // Wait untill main sends data
        signalFromMain.wait();

        // after the wait, we own the lock.
        std::cout<<"Worker thread is processing data\n";
        data += " after processing";

        // Send data back to main()
        std::cout<<"Worker thread signals data processing completed\n";
        signalMain.set_value();
    }

    worker(std::promise<void>& p) {
        signalFromMain = p.get_future();
        thread = std::thread(&worker::worker_thread, this);
    }
};

int main() {
    std::promise<void> signalWorker;
    worker w(signalWorker);

    w.data = "Example data";
    std::cout<<" Worker data before processing: "<< w.data<<std::endl;
    //send data to worker thread
    {
        std::cout<<"main() signal data ready for processing\n";
        signalWorker.set_value();
    }

    w.signalMain.get_future().wait();
    // wait for worker

    std::cout<<"Back in main(), data = "<<w.data<<"\n";

    w.thread.join();
}

