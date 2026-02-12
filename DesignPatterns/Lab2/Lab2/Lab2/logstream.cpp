#include "logger.h"

logstream::Message::Message(logstream& parent, LogLevel level)
    : parent_(parent), level_(level), closed_(false) {
}

// Конструктор перемещения: забирает строковый поток и помечает исходный как закрытый
logstream::Message::Message(Message&& other) noexcept
    : parent_(other.parent_), level_(other.level_),
    ss_(std::move(other.ss_)), closed_(other.closed_) {
    other.closed_ = true;
}

logstream::Message::~Message() {
    if (!closed_) flush_on_destroy();
}

// Оператор для манипуляторов: если std::endl – немедленно отправляем строку
logstream::Message& logstream::Message::operator<<(std::ostream& (*manip)(std::ostream&)) {
    typedef std::ostream& (*manip_t)(std::ostream&);
    if (manip == static_cast<manip_t>(std::endl)) {
        ss_ << manip;                          // добавляет '\n'
        std::string s = ss_.str();
        ss_.str("");
        LogManager::instance().write(level_, parent_.prefix_, s, parent_.at_line_start_, true);
        closed_ = true;
    }
    else {
        ss_ << manip;
    }
    return *this;
}

// Принудительный сброс сообщения (можно добавить перевод строки)
void logstream::Message::flush(bool force_newline) {
    if (closed_) return;
    std::string s = ss_.str();
    ss_.str("");
    if (!s.empty() || force_newline) {
        if (force_newline) s += "\n";
        LogManager::instance().write(level_, parent_.prefix_, s, parent_.at_line_start_, true);
    }
    closed_ = true;
}

// Сброс остатка сообщения при разрушении (без принудительного flush)
void logstream::Message::flush_on_destroy() {
    if (closed_) return;
    std::string leftover = ss_.str();
    ss_.str("");
    if (!leftover.empty()) {
        LogManager::instance().write(level_, parent_.prefix_, leftover, parent_.at_line_start_, false);
    }
    closed_ = true;
}

// Конструктор: запоминает префикс и увеличивает счётчик ссылок
logstream::logstream(const std::string& prefix)
    : prefix_(prefix), at_line_start_(true) {
    LogManager::instance().add_ref();
}

// Деструктор: уменьшает счётчик ссылок
logstream::~logstream() {
    LogManager::instance().release_ref();
}

void logstream::set_prefix(const std::string& p) {
    prefix_ = p;
}

const std::string& logstream::get_prefix() const {
    return prefix_;
}

void logstream::set_log_file(const std::string& name) {
    LogManager::instance().set_file(name);
}

void logstream::set_level(LogLevel lvl) {
    LogManager::instance().set_level(lvl);
}

LogLevel logstream::get_level() {
    return LogManager::instance().get_level();
}