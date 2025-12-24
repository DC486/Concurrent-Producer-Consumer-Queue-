# Concurrent Producer–Consumer Queue in C++

This project demonstrates a basic **producer–consumer pipeline** implemented in **C++17** using standard concurrency primitives. The goal is to understand **thread synchronization**, **safe shared data access**, and **performance measurement** in a multithreaded environment.

---

## 🧠 Core Idea

A **producer thread** generates data and pushes it into a shared queue, while a **consumer thread** safely retrieves and processes the data. Access to the shared queue is synchronized to prevent race conditions.

---

## 🛠️ Implementation Details

- **Threads:** `std::thread`
- **Synchronization:** `std::mutex`, `std::condition_variable`
- **Shared Data Structure:** `std::queue`
- **Timing & Measurement:** `std::chrono`

---

## 🔒 Why `mutex` and `condition_variable`?

- A **mutex** ensures exclusive access to the shared queue, preventing data races.
- A **condition_variable** allows the consumer to sleep when the queue is empty, avoiding busy waiting and unnecessary CPU usage.

---

## ⚙️ Design Observations

- Removing synchronization leads to **race conditions and undefined behavior**.
- Replacing `condition_variable` with busy waiting significantly increases **CPU usage**.
- This implementation favors **correctness and clarity** over lock-free complexity, making it suitable for learning and debugging.

---

## ⏱️ Performance Measurement

The total execution time for processing a fixed number of items is measured using high-resolution clocks. This helps observe the overhead introduced by synchronization primitives.

---

## ▶️ How to Build and Run

```bash
g++ -std=c++17 producer_consumer.cpp -pthread -O2
./a.out
