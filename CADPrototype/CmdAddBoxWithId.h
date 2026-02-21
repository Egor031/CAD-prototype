#pragma once

#include "ICommand.h"
#include "Document.h"

#include <TopoDS_Shape.hxx>

class CmdAddBoxWithId : public ICommand
{
public:
    CmdAddBoxWithId(EntityId id, double dx, double dy, double dz);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "AddBoxWithId"; }
    std::string ToJson() const override;

private:
    EntityId myId = 0;
    double myDx = 0, myDy = 0, myDz = 0;
    bool myApplied = false; // если id занят и команда не применилась

    TopoDS_Shape myCreated; // чтобы Undo был стабильным даже если геом генерится
    bool myHasCreated = false;
};