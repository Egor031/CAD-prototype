#pragma once

#include "ICommand.h"
#include "Document.h"

#include <string>

class CmdAddCircle : public ICommand
{
public:
    CmdAddCircle(double cx, double cy, double r);
    CmdAddCircle(EntityId id, double cx, double cy, double r);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "AddCircle"; }
    std::string ToJson() const override;

private:
    EntityId myCreatedId = 0;

    double myCx = 0;
    double myCy = 0;
    double myR = 0;
};