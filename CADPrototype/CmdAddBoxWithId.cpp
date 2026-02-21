#include "CmdAddBoxWithId.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <sstream>

CmdAddBoxWithId::CmdAddBoxWithId(EntityId id, double dx, double dy, double dz)
    : myId(id), myDx(dx), myDy(dy), myDz(dz)
{
}

void CmdAddBoxWithId::Apply(Document& doc)
{
    // создаём геометрию
    myCreated = BRepPrimAPI_MakeBox(myDx, myDy, myDz).Shape();
    myHasCreated = true;

    myApplied = doc.AddShapeWithId(myId, myCreated);
}

void CmdAddBoxWithId::Undo(Document& doc)
{
    if (!myApplied)
        return;

    doc.RemoveShape(myId);
}

std::string CmdAddBoxWithId::ToJson() const
{
    std::ostringstream ss;
    ss << "{"
        << "\"type\":\"AddBoxWithId\","
        << "\"id\":" << myId << ","
        << "\"dx\":" << myDx << ","
        << "\"dy\":" << myDy << ","
        << "\"dz\":" << myDz
        << "}";
    return ss.str();
}