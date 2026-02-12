#include "logger.h"

int main() {
    logstream::set_log_file("test.log");
    logstream::set_level(LogLevel::DEBUG);

    logstream lg("main");

    lg.debug() << "debug message: x=" << 42 << std::endl;
    lg.info() << "app started" << std::endl;
    lg.at(LogLevel::WARNING) << "low memory: " << 123 << std::endl;
    lg.error() << "fatal error code " << -1 << std::endl;

    return 0;
}