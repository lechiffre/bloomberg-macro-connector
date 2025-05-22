#ifndef _BLPCONN_PROFILER_H
#define _BLPCONN_PROFILER_H

#include <climits>
#include <cstdint>
#include <mutex>
#include <thread>
#include <string>

constexpr uint32_t QUEUE_SIZE = 128;

namespace BlpConn {

struct ProfileEvent {
    char module[32];
    char metric[32];
    char message[256];
    uint64_t event_id;
};

/**
  * It receives performance events from any function.
  * It returns a uint64_t id that can be used in following events
  * to make relationships between them.
  * When a new event is received, it doesn't block the main thread.
  * If the queue is full, it will drop the event.
  * The queue is a circular buffer of size QUEUE_SIZE.
  * The queue is protected by a mutex
  * An asynchronous thread is responsible for writing the events to a file.
  * The file is opened in append mode and is closed when the program ends.
 */
class ProfileQueue {
public:
    ProfileQueue(const std::string& output_file = "profiler.txt");
    ~ProfileQueue();

    // Add a new event to the queue, returns the event_id assigned
    uint64_t push(const char* module, const char* metric, const char* message,
                  uint64_t event_id = 0);

    // Called by the writer thread to write events to file
    void writerLoop();

    // Returns true if the queue is full
    bool isFull();

    // Returns true if the queue is empty
    bool isEmpty();

    // Stop the writer thread and flush remaining events
    void stop();

private:
    ProfileEvent queue_[QUEUE_SIZE];
    uint32_t head_ = 0;
    uint32_t tail_ = 0;
    uint32_t count_ = 0;
    std::mutex mtx_;
    std::thread writer_thread_;
    bool running_ = false;
    std::string output_file_;
    uint64_t next_event_id_ = 1;
};

} // namespace BlpConn

#endif // _BLPCONN_PROFILER_H