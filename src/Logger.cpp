#include "Logger.hpp"

#include<iostream>
#include<iomanip>
#include<chrono>
#include<ctime>

void Logger::Log(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
     
    std::cout << "LOG: [ " << 
        std::put_time(std::localtime(&currentTime), "%D %T") <<
        " ]  " << message << std::endl;
}

void Logger::Err(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
    
    std::cerr << "ERR: [ " <<
        std::put_time(std::localtime(&currentTime), "%D %T") <<
        " ]  " << message << std::endl;
}
