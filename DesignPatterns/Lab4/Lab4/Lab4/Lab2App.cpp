#define _CRT_SECURE_NO_WARNINGS
#include "Lab2App.h"
#include <chrono>
#include <iomanip>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

LogManager& LogManager::instance() {
    static LogManager mgr;
    return mgr;
}

LogManager::LogManager() : refcount_(0), level_(LogLevel::INFO) {}

LogManager::~LogManager() {
    if (ofs_.is_open()) {
        ofs_.flush();
        ofs_.close();
    }
}

void LogManager::open_stream_nolock() {
    if (!filename_.empty()) {
        ofs_.open(filename_, std::ios::out | std::ios::app);
        if (!ofs_.is_open()) {
            std::cerr << "Logger: failed to open file '" << filename_ << "'\n";
        }
    }
}

void LogManager::set_file(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (ofs_.is_open()) {
        ofs_.flush();
        ofs_.close();
    }
    filename_ = filename;
    if (refcount_ > 0) open_stream_nolock();
}

void LogManager::set_level(LogLevel lvl) {
    std::lock_guard<std::mutex> lock(mtx_);
    level_ = lvl;
}

LogLevel LogManager::get_level() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return level_;
}

void LogManager::add_ref() {
    std::lock_guard<std::mutex> lock(mtx_);
    ++refcount_;
    if (refcount_ == 1 && !filename_.empty()) open_stream_nolock();
}

void LogManager::release_ref() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (refcount_ > 0) --refcount_;
    if (refcount_ == 0) {
        if (ofs_.is_open()) {
            ofs_.flush();
            ofs_.close();
        }
    }
}

void LogManager::write(LogLevel msg_level,
    const std::string& prefix,
    const std::string& data,
    bool& at_line_start,
    bool flush_on_end) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (msg_level < level_) return;

    if (!ofs_.is_open() && !filename_.empty())
        open_stream_nolock();

    std::ostream& out = ofs_.is_open()
        ? static_cast<std::ostream&>(ofs_)
        : static_cast<std::ostream&>(std::cerr);

    std::string full_pref;
    if (!prefix.empty())
        full_pref = std::string("[") + to_string(msg_level) + "] " + prefix + " ";
    else
        full_pref = std::string("[") + to_string(msg_level) + "] ";

    std::size_t pos = 0;
    while (pos < data.size()) {
        if (at_line_start) {
            out << full_pref;
            at_line_start = false;
        }
        auto next_n = data.find('\n', pos);
        if (next_n == std::string::npos) {
            out << data.substr(pos);
            pos = data.size();
        }
        else {
            out << data.substr(pos, next_n - pos + 1);
            pos = next_n + 1;
            at_line_start = true;
        }
    }

    if (flush_on_end) out.flush();
}

logstream::logstream(const std::string& prefix)
    : prefix_(prefix), current_level_(LogLevel::INFO), at_line_start_(true) {
    LogManager::instance().add_ref();
}

logstream::~logstream() {
    if (!buffer_.str().empty()) {
        flush_buffer();
    }
    LogManager::instance().release_ref();
}

void logstream::set_log_file(const std::string& filename) {
    LogManager::instance().set_file(filename);
}

void logstream::set_level(LogLevel lvl) {
    LogManager::instance().set_level(lvl);
}

logstream& logstream::operator<<(std::ostream& (*manip)(std::ostream&)) {
    buffer_ << manip;
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl) ||
        manip == static_cast<std::ostream& (*)(std::ostream&)>(std::flush)) {
        flush_buffer();
    }
    return *this;
}

logstream& logstream::at(LogLevel lvl) {
    flush_buffer();
    current_level_ = lvl;
    return *this;
}

logstream& logstream::debug() { return at(LogLevel::DEBUGG); }
logstream& logstream::info() { return at(LogLevel::INFO); }
logstream& logstream::warning() { return at(LogLevel::WARNING); }
logstream& logstream::error() { return at(LogLevel::EERROR); }

void logstream::flush_buffer() {
    std::string data = buffer_.str();
    if (!data.empty()) {
        LogManager::instance().write(current_level_, prefix_, data, at_line_start_, true);
        buffer_.str("");
        buffer_.clear();
    }
}

Lab2App::Lab2App() : logFile_("lab2.log") {
}

Lab2App::~Lab2App() {
}

void Lab2App::SetLogFile(const std::string& filename) {
    logFile_ = filename;
}

std::vector<std::string> Lab2App::GetLogs() const {
    return capturedLogs_;
}

void Lab2App::CaptureLog(const std::string& message) {
    capturedLogs_.push_back(message);
}

void Lab2App::Run() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    RunDemo();
}

void Lab2App::RunDemo() {
    logstream::set_log_file(logFile_);
    logstream::set_level(LogLevel::DEBUGG);

    logstream lg("main");

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream timeStr;
    timeStr << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

    std::cout << "ЛАБОРАТОРНАЯ 2: ПАТТЕРН ОДИНОЧКА" << std::endl;
    std::cout << "(Потокобезопасный логгер)" << std::endl;
    std::cout << std::endl;

    std::cout << "Файл логов: " << logFile_ << std::endl;
    std::cout << "Уровень логирования: DEBUG" << std::endl;
    std::cout << std::endl;

    std::cout << "Начало записи логов" << std::endl;

    lg.debug() << "debug message: x=" << 42 << std::endl;
    CaptureLog("[" + timeStr.str() + "] [DEBUG] main debug message: x=42");

    lg.info() << "application started successfully" << std::endl;
    CaptureLog("[" + timeStr.str() + "] [INFO] main application started successfully");

    lg.at(LogLevel::WARNING) << "low memory warning: " << 123 << " MB remaining" << std::endl;
    CaptureLog("[" + timeStr.str() + "] [WARNING] main low memory warning: 123 MB remaining");

    lg.error() << "fatal error code " << -1 << ": connection failed" << std::endl;
    CaptureLog("[" + timeStr.str() + "] [ERROR] main fatal error code -1: connection failed");

    std::cout << "Конец записи логов" << std::endl;
    std::cout << std::endl;

    std::cout << "Записанные сообщения:" << std::endl;
    for (const auto& log : capturedLogs_) {
        std::cout << "  " << log << std::endl;
    }

    std::cout << std::endl;
}
