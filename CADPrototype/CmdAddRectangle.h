#pragma once

#include "ICommand.h"
#include "Document.h"

#include <string>

class CmdAddRectangle : public ICommand
{
public:
    CmdAddRectangle(double x, double y, double w, double h);
    CmdAddRectangle(EntityId id, double x, double y, double w, double h);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "AddRectangle"; }
    std::string ToJson() const override;

private:
    EntityId myId = 0;

    double myX = 0;
    double myY = 0;
    double myW = 0;
    double myH = 0;
};