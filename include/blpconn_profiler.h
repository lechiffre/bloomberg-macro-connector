#ifndef _BLPCONN_PROFILER_H
#define _BLPCONN_PROFILER_H

#include <spdlog/spdlog.h>
// #include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/file_sinks.h>

extern bool __is_profiling;

#ifdef ENABLE_PROFILING

inline std::shared_ptr<spdlog::logger> get_perf_logger() {
    static std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("perf_logger", "profiler.txt");
    return logger;
}

#define PROFILE_FUNCTION() \
    auto __start_time = std::chrono::high_resolution_clock::now(); \
    const char * __function_name = __func__;

#define END_PROFILE_FUNCTION() \
    { \
        if (__is_profiling) { \
            auto __end_time = std::chrono::high_resolution_clock::now(); \
            auto __duration = std::chrono::duration_cast<std::chrono::microseconds>(__end_time - __start_time).count(); \
            get_perf_logger()->info("{} {} microseconds", __function_name, __duration); \
        } \
    }

#else

#define PROFILE_FUNCTION()
#define END_PROFILE_FUNCTION()

#endif

#endif // _BLPCONN_PROFILER_H
