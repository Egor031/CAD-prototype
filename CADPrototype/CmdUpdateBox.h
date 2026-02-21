#pragma once

#include "ICommand.h"
#include "Document.h"

class CmdUpdateBox : public ICommand
{
public:
    CmdUpdateBox(EntityId id, double dx, double dy, double dz);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "UpdateBox"; }
    std::string ToJson() const override;

private:
    EntityId myId = 0;
    double myNewDx = 0, myNewDy = 0, myNewDz = 0;

    bool myHasOld = false;
    double myOldDx = 0, myOldDy = 0, myOldDz = 0;

    bool myApplied = false;
};