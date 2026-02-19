#pragma once
#include <string>

class Document;

class ICommand
{
public:
    virtual ~ICommand() = default;

    virtual void Apply(Document& doc) = 0;
    virtual void Undo(Document& doc) = 0;

    virtual std::string Name() const = 0;

    // Машиночитаемое описание команды (для истории/LLM)
    virtual std::string ToJson() const = 0;
};
