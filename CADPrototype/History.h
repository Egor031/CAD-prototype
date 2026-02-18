#pragma once

#include <memory>
#include <vector>


class ICommand;
class Document;

class History
{
public:
    void Apply(std::unique_ptr<ICommand> cmd, Document& doc);

    void Undo(Document& doc);
    void Redo(Document& doc);

private:
    std::vector<std::unique_ptr<ICommand>> myUndoStack;
    std::vector<std::unique_ptr<ICommand>> myRedoStack;
};
