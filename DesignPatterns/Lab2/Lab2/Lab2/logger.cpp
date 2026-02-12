#include "logger.h"

// Возвращает единственный экземпляр
LogManager& LogManager::instance() {
    static LogManager mgr;
    return mgr;
}

// Конструктор: обнуляет счётчик, уровень по умолчанию INFO
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