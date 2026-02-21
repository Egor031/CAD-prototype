#include "CmdDeleteEntity.h"

#include <sstream>

CmdDeleteEntity::CmdDeleteEntity(EntityId id)
    : myId(id)
{
}

void CmdDeleteEntity::Apply(Document& doc)
{
    myHasBackup = doc.TryGetTopoShape(myId, myBackup);
    doc.RemoveShape(myId);
}

void CmdDeleteEntity::Undo(Document& doc)
{
    if (!myHasBackup)
        return;

    // восстановим с тем же id
    doc.AddShapeWithId(myId, myBackup);
}

std::string CmdDeleteEntity::ToJson() const
{
    std::ostringstream ss;
    ss << "{"
        << "\"type\":\"DeleteEntity\","
        << "\"id\":" << myId
        << "}";
    return ss.str();
}
