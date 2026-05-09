#include "CmdDeleteEntity.h"

#include <sstream>

CmdDeleteEntity::CmdDeleteEntity(EntityId id)
    : myId(id)
{
}

void CmdDeleteEntity::Apply(Document& doc)
{
    if (!myHasBackup)
        myHasBackup = doc.TryGetTopoShape(myId, myBackup);

    if (!myHasMeta)
        myHasMeta = doc.TryGetMeta(myId, myBackupMeta);

    doc.RemoveShape(myId);
}

void CmdDeleteEntity::Undo(Document& doc)
{
    if (!myHasBackup)
        return;

    if (myHasMeta)
        doc.AddShapeWithIdAndMeta(myId, myBackup, myBackupMeta);
    else
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
