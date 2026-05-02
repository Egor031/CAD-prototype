#pragma once

#include "ICommand.h"
#include "Document.h"

#include <string>

class CmdAddLine : public ICommand
{
public:
    CmdAddLine(double x1, double y1, double x2, double y2);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override;
    std::string ToJson() const override;

private:
    EntityId myId = 0;

    double myX1 = 0;
    double myY1 = 0;
    double myX2 = 0;
    double myY2 = 0;
};