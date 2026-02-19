#pragma once

#include "ICommand.h"
#include "Document.h"

class CmdAddBox : public ICommand
{
public:
    CmdAddBox(double dx, double dy, double dz);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "AddBox"; }
    std::string ToJson() const override;

private:
    double myDx, myDy, myDz;
    EntityId myCreatedId = 0;
};
