#include "CmdUpdateBox.h"

#include <sstream>

CmdUpdateBox::CmdUpdateBox(EntityId id, double dx, double dy, double dz)
    : myId(id), myNewDx(dx), myNewDy(dy), myNewDz(dz)
{
}

void CmdUpdateBox::Apply(Document& doc)
{
    myHasOld = doc.TryGetBoxParams(myId, myOldDx, myOldDy, myOldDz);
    myApplied = doc.UpdateBox(myId, myNewDx, myNewDy, myNewDz);
}

void CmdUpdateBox::Undo(Document& doc)
{
    if (!myApplied || !myHasOld)
        return;

    doc.UpdateBox(myId, myOldDx, myOldDy, myOldDz);
}

std::string CmdUpdateBox::ToJson() const
{
    std::ostringstream ss;
    ss << "{"
        << "\"type\":\"UpdateBox\","
        << "\"id\":" << (unsigned long long)myId << ","
        << "\"dx\":" << myNewDx << ","
        << "\"dy\":" << myNewDy << ","
        << "\"dz\":" << myNewDz
        << "}";
    return ss.str();
}