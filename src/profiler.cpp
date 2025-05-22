/**
 * @file profiler.cpp
 * @brief A profiler function to enqueue performance events.
 *
 * It receive performance events and manage to write them to a file
 * without blocking the main thread.
 * The structure of the profile event is:
 */

#include <fstream>
#include <cstring>
#include <mutex>
#include <thread>
#include <chrono>
#include "blpconn_message.h"
#include "blpconn_profiler.h"
#include "blpconn_deserialize.h"

namespace BlpConn {

ProfileQueue::ProfileQueue(const std::string& output_file)
    : head_(0), tail_(0), count_(0), running_(true), output_file_(output_file), next_event_id_(1) {
    std::lock_guard<std::mutex> lock(mtx_);
    writer_thread_ = std::thread(&ProfileQueue::writerLoop, this);
}

ProfileQueue::~ProfileQueue() {
    stop();
}

uint64_t ProfileQueue::push(const char* module, const char* metric, const char* message, uint64_t event_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (isFull()) {
        // Drop event if full
        return 0;
    }
    if (event_id == 0) {
        event_id = next_event_id_++;
    }
    ProfileEvent& evt = queue_[tail_];
    std::strncpy(evt.module, module, sizeof(evt.module) - 1);
    evt.module[sizeof(evt.module) - 1] = '\0';
    std::strncpy(evt.metric, metric, sizeof(evt.metric) - 1);
    evt.metric[sizeof(evt.metric) - 1] = '\0';
    std::strncpy(evt.message, message, sizeof(evt.message) - 1);
    evt.message[sizeof(evt.message) - 1] = '\0';
    evt.event_id = event_id;;
    tail_ = (tail_ + 1) % QUEUE_SIZE;
    ++count_;
    return event_id;
}

bool ProfileQueue::isFull() {
    return count_ == QUEUE_SIZE;
}

bool ProfileQueue::isEmpty() {
    return count_ == 0;
}

void ProfileQueue::stop() {
    if (!running_) return;
    std::lock_guard<std::mutex> lock(mtx_);
    running_ = false;
    if (writer_thread_.joinable()) {
        writer_thread_.join();
    }
}

void ProfileQueue::writerLoop() {
    std::ofstream ofs(output_file_, std::ios::app);
    while (running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (isEmpty()) continue;
        std::lock_guard<std::mutex> lock(mtx_);
        ProfileEvent evt;
        DateTimeType dt = currentTime();
        evt = queue_[head_];
        head_ = (head_ + 1) % QUEUE_SIZE;
        --count_;
        ofs << dt.microseconds << "|" << evt.event_id << "|"
            << evt.module << "|" << evt.metric << "|" << evt.message << "\n";
        ofs.flush();
    }
    ofs.close();
}

} // namespace BlpConn