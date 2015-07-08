#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

struct worker {
    std::mutex m;
    std::condition_variable cv;
    std::string data;
    bool ready = false;
    bool processed = false;
    std::thread thread;

    void worker_thread() {
        // Wait untill main sends data
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&]{ return ready; });

        // after the wait, we own the lock.
        std::cout<<"Worker thread is processing data\n";
        data += " after processing";

        // Send data back to main()
        processed = true;
        std::cout<<"Worker thread signals data processing completed\n";

        // Manual unlocking is done before notifying, to avoid waking up the waiting thread only to block again
        lock.unlock();
        cv.notify_one();
    }

    worker() {
        thread = std::thread(&worker::worker_thread, this);
    }
};

int main() {
    worker w;

    w.data = "Example data";

    std::cout<<" Worker data before processing: "<< w.data<<std::endl;

    //send data to worker thread
    {
        std::lock_guard<std::mutex> lock(w.m);
        w.ready = true;
        std::cout<<"main() signal data ready for processing\n";
    }
    w.cv.notify_one();

    // wait for worker
    {
        std::unique_lock<std::mutex> lock(w.m);
        w.cv.wait(lock, [&]{ return w.processed; });
    }

    std::cout<<"Back in main(), data = "<<w.data<<"\n";

    w.thread.join();
}

