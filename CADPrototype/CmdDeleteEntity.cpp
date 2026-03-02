#include "CmdDeleteEntity.h"

#include <sstream>

CmdDeleteEntity::CmdDeleteEntity(EntityId id)
    : myId(id)
{
}

void CmdDeleteEntity::Apply(Document& doc)
{
    myHasBackup = doc.TryGetTopoShape(myId, myBackup);

    // метаданные тоже сохраняем
    myHasMeta = doc.TryGetMeta(myId, myBackupKind, myBackupDx, myBackupDy, myBackupDz);

    doc.RemoveShape(myId);
}

void CmdDeleteEntity::Undo(Document& doc)
{
    if (!myHasBackup)
        return;

    if (myHasMeta)
    {
        doc.AddShapeWithIdAndMeta(myId, myBackup, myBackupKind, myBackupDx, myBackupDy, myBackupDz);
    }
    else
    {
        // fallback
        doc.AddShapeWithId(myId, myBackup);
    }
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
