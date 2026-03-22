#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <vector>

enum class LogLevel { DEBUGG = 0, INFO = 1, WARNING = 2, EERROR = 3 };

inline std::string to_string(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::DEBUGG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::EERROR: return "ERROR";
    }
    return "UNKNOWN";
}

class LogManager {
public:
    static LogManager& instance();

    void set_file(const std::string& filename);
    void set_level(LogLevel lvl);
    LogLevel get_level() const;

    void add_ref();
    void release_ref();

    void write(LogLevel msg_level,
               const std::string& prefix,
               const std::string& data,
               bool& at_line_start,
               bool flush_on_end);

private:
    LogManager();
    ~LogManager();

    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    void open_stream_nolock();

    mutable std::mutex mtx_;
    std::ofstream ofs_;
    std::string filename_;
    int refcount_;
    LogLevel level_;
};

class logstream {
public:
    explicit logstream(const std::string& prefix = "");
    ~logstream();

    logstream(const logstream&) = delete;
    logstream& operator=(const logstream&) = delete;

    logstream(logstream&&) noexcept = default;
    logstream& operator=(logstream&&) noexcept = default;

    static void set_log_file(const std::string& filename);
    static void set_level(LogLevel lvl);

    template<typename T>
    logstream& operator<<(const T& value) {
        buffer_ << value;
        if (buffer_.str().find('\n') != std::string::npos) {
            flush_buffer();
        }
        return *this;
    }

    logstream& operator<<(std::ostream& (*manip)(std::ostream&));

    logstream& at(LogLevel lvl);
    logstream& debug();
    logstream& info();
    logstream& warning();
    logstream& error();

private:
    void flush_buffer();

    std::string prefix_;
    std::ostringstream buffer_;
    LogLevel current_level_;
    bool at_line_start_;
};

class Lab2App {
public:
    Lab2App();
    ~Lab2App();

    void Run();
    void SetLogFile(const std::string& filename);
    std::vector<std::string> GetLogs() const;

private:
    std::string logFile_;
    std::vector<std::string> capturedLogs_;

    void RunDemo();
    void CaptureLog(const std::string& message);
};
