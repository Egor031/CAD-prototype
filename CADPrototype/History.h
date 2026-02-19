#pragma once

#include <memory>
#include <string>
#include <vector>

class ICommand;
class Document;

class History
{
public:
    void Apply(std::unique_ptr<ICommand> cmd, Document& doc);

    void Undo(Document& doc);
    void Redo(Document& doc);

    // История построения (только то, что применено и не отменено)
    std::string ExportJson() const;

    // (опционально) очистить историю
    void Clear();

private:
    std::vector<std::unique_ptr<ICommand>> myUndoStack;
    std::vector<std::unique_ptr<ICommand>> myRedoStack;

    // JSON каждой команды, которая сейчас в undo-стеке (синхронизируем)
    std::vector<std::string> myAppliedJson;
};
