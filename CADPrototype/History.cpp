#include "History.h"
#include "ICommand.h"
#include "Document.h"
#include <utility>  // std::move
#include <memory>   // std::make_unique, unique_ptr


void History::Apply(std::unique_ptr<ICommand> cmd, Document& doc)
{
    cmd->Apply(doc);
    myUndoStack.push_back(std::move(cmd));
    myRedoStack.clear();
}

void History::Undo(Document& doc)
{
    if (myUndoStack.empty())
        return;

    auto cmd = std::move(myUndoStack.back());
    myUndoStack.pop_back();

    cmd->Undo(doc);
    myRedoStack.push_back(std::move(cmd));
}

void History::Redo(Document& doc)
{
    if (myRedoStack.empty())
        return;

    auto cmd = std::move(myRedoStack.back());
    myRedoStack.pop_back();

    cmd->Apply(doc);
    myUndoStack.push_back(std::move(cmd));
}
