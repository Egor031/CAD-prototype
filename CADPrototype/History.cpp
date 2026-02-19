#include "History.h"
#include "ICommand.h"
#include "Document.h"

#include <sstream>

void History::Apply(std::unique_ptr<ICommand> cmd, Document& doc)
{
    cmd->Apply(doc);

    // добавляем в историю построения
    myAppliedJson.push_back(cmd->ToJson());

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

    // откатываем историю построения
    if (!myAppliedJson.empty())
        myAppliedJson.pop_back();

    myRedoStack.push_back(std::move(cmd));
}

void History::Redo(Document& doc)
{
    if (myRedoStack.empty())
        return;

    auto cmd = std::move(myRedoStack.back());
    myRedoStack.pop_back();

    cmd->Apply(doc);

    // возвращаем в историю построения
    myAppliedJson.push_back(cmd->ToJson());

    myUndoStack.push_back(std::move(cmd));
}

std::string History::ExportJson() const
{
    std::ostringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < myAppliedJson.size(); ++i)
    {
        ss << "  " << myAppliedJson[i];
        if (i + 1 < myAppliedJson.size())
            ss << ",";
        ss << "\n";
    }
    ss << "]";
    return ss.str();
}

void History::Clear()
{
    myUndoStack.clear();
    myRedoStack.clear();
    myAppliedJson.clear();
}
