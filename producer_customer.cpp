#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Shared queue acting as the buffer between producer and consumer
std::queue<int> buffer;

// Mutex to protect shared access to the queue
std::mutex mtx;

// Condition variable to coordinate producer and consumer
std::condition_variable cv;

// Total number of items to produce
const int MAX_ITEMS = 100000;

// Flag to indicate producer has finished producing data
bool finished = false;

/*
 * Producer function
 * -----------------
 * Generates integers from 1 to MAX_ITEMS and pushes them into the shared queue.
 * Uses mutex to ensure thread-safe access to the queue.
 * Notifies the consumer whenever new data is available.
 */
void producer() {
    for (int i = 1; i <= MAX_ITEMS; ++i) {
        // Lock the mutex before accessing the shared queue
        std::unique_lock<std::mutex> lock(mtx);
        buffer.push(i);          // Push item into the queue
        lock.unlock();           // Unlock early to reduce contention

        // Notify one waiting consumer thread that data is available
        cv.notify_one();
    }

    // Signal to the consumer that production is complete
    std::unique_lock<std::mutex> lock(mtx);
    finished = true;
    lock.unlock();

    // Notify consumer in case it is waiting and no more data will come
    cv.notify_one();
}

/*
 * Consumer function
 * -----------------
 * Continuously waits for data to become available in the queue.
 * Processes items safely using synchronization primitives.
 * Terminates when producer is finished and the queue is empty.
 */
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait until there is data in the queue or production is finished
        cv.wait(lock, [] {
            return !buffer.empty() || finished;
        });

        // Consume all available items
        while (!buffer.empty()) {
            int item = buffer.front();
            buffer.pop();
            // Simulate processing of the item (no actual work here)
        }

        // Exit condition: no more data will arrive
        if (finished && buffer.empty()) {
            break;
        }
    }
}

int main() {
    // Start timing execution
    auto start = std::chrono::high_resolution_clock::now();

    // Create producer and consumer threads
    std::thread prodThread(producer);
    std::thread consThread(consumer);

    // Wait for both threads to complete
    prodThread.join();
    consThread.join();

    // Stop timing execution
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Output total execution time
    std::cout << "Processing completed in "
              << duration.count()
              << " seconds." << std::endl;

    return 0;
}
