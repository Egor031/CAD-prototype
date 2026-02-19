#pragma once

#include <memory>
#include <string>
#include <vector>

class ICommand;

class CommandFactory
{
public:
    // Парсит JSON-массив команд: [ {...}, {...} ]
    // Возвращает список команд или пустой список + error.
    static std::vector<std::unique_ptr<ICommand>> ParseCommandArray(
        const std::string& json,
        std::string& error);
};
