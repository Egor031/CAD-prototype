#pragma once

#include "ICommand.h"
#include "Document.h"
#include <string>

#include <TopoDS_Shape.hxx>

class CmdDeleteEntity : public ICommand
{
public:
    explicit CmdDeleteEntity(EntityId id);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "DeleteEntity"; }
    std::string ToJson() const override;

    ShapeMeta myBackupMeta;
    bool myHasMeta = false;
private:
    EntityId myId = 0;

    // бэкап для Undo
    TopoDS_Shape myBackup;
    bool myHasBackup = false;
};
