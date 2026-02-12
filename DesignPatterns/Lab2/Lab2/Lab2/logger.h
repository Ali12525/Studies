#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <memory>
#include <iostream>
#include <iomanip>

enum class LogLevel : int {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARNING = 3,
    ERROR = 4,
    FATAL = 5
};

// Преобразовать уровень в строку
inline const char* to_string(LogLevel lvl) {
    switch (lvl) {
    case LogLevel::TRACE:   return "TRACE";
    case LogLevel::DEBUG:   return "DEBUG";
    case LogLevel::INFO:    return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR:   return "ERROR";
    case LogLevel::FATAL:   return "FATAL";
    }
    return "UNKNOWN";
}

// LogManager – управляет файлом и уровнем логирования (одиночка)
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
    void open_stream_nolock();

    mutable std::mutex mtx_;
    std::ofstream ofs_;             // выходной файловый поток
    std::string filename_;
    size_t refcount_;
    LogLevel level_;
};

//  logstream – основной интерфейс для логирования
class logstream {
public:
    class Message {
    public:
        Message(logstream& parent, LogLevel level);     // Конструктор сообщения
        Message(Message&& other) noexcept;              // Конструктор перемещения
        Message& operator=(Message&&) = delete;         // Запрет копирования
        Message(const Message&) = delete;
        Message& operator=(const Message&) = delete;
        ~Message();

        // Оператор вывода для любого типа
        template <typename T>
        inline Message& operator << (const T& rhs) {
            ss_ << rhs;
            return *this;
        }

        // Обработка манипуляторов потока (endl и т.п.)
        Message& operator << (std::ostream& (*manip)(std::ostream&));

        // Принудительный сброс сообщения в лог
        void flush(bool force_newline = false);

    private:
        void flush_on_destroy();

        logstream& parent_;
        LogLevel level_;
        std::ostringstream ss_;             // накопительный буфер
        bool closed_;                       // флаг: сообщение уже отправлено
    };

    // Конструктор с префиксом
    explicit logstream(const std::string& prefix = std::string());
    ~logstream();

    inline Message at(LogLevel lvl) { return Message(*this, lvl); }
    inline Message trace() { return at(LogLevel::TRACE); }
    inline Message debug() { return at(LogLevel::DEBUG); }
    inline Message info() { return at(LogLevel::INFO); }
    inline Message warning() { return at(LogLevel::WARNING); }
    inline Message error() { return at(LogLevel::ERROR); }
    inline Message fatal() { return at(LogLevel::FATAL); }

    static void set_log_file(const std::string& name);
    static void set_level(LogLevel lvl);
    static LogLevel get_level();

    void set_prefix(const std::string& p);
    const std::string& get_prefix() const;

private:
    std::string prefix_;
    bool at_line_start_;
};