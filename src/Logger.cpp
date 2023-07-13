#include "Logger.hpp"

#include<iostream>
#include<iomanip>
#include<chrono>
#include<ctime>
#include<termcolor/termcolor.hpp>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString() {
    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
    
    char output[std::size("mm/dd/yyyy hh:mm::ss")];

    std::strftime(
            std::data(output),
            std::size(output),
            "%D %T", 
            std::localtime(&currentTime));

    return output;
}

void Logger::Log(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG: [ " + CurrentDateTimeToString() + " ]  " + message; 
    std::cout << termcolor::green <<
        logEntry.message <<
        termcolor::reset << std::endl;
    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR: [ " + CurrentDateTimeToString() + " ]  " + message; 
    std::cerr << termcolor::red << 
        logEntry.message <<
        termcolor::reset << std::endl;
    messages.push_back(logEntry);
}
