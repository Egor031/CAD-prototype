#include "CmdAddRectangle.h"

#include <sstream>

CmdAddRectangle::CmdAddRectangle(double x, double y, double w, double h)
    : myX(x), myY(y), myW(w), myH(h)
{}

CmdAddRectangle::CmdAddRectangle(EntityId id, double x, double y, double w, double h)
    : myId(id), myX(x), myY(y), myW(w), myH(h)
{}

void CmdAddRectangle::Apply(Document& doc)
{
    if (myId == 0)
        myId = doc.AddRectangle(myX, myY, myW, myH);
    else
        doc.AddRectangleWithId(myId, myX, myY, myW, myH);
}

void CmdAddRectangle::Undo(Document& doc)
{
    if (myId != 0)
        doc.RemoveShape(myId);
}

std::string CmdAddRectangle::ToJson() const
{
    std::ostringstream ss;

    if (myId != 0)
    {
        ss << "{\"type\":\"AddRectangleWithId\""
            << ",\"id\":" << myId
            << ",\"x\":" << myX
            << ",\"y\":" << myY
            << ",\"w\":" << myW
            << ",\"h\":" << myH
            << "}";
    }
    else
    {
        ss << "{\"type\":\"AddRectangle\""
            << ",\"x\":" << myX
            << ",\"y\":" << myY
            << ",\"w\":" << myW
            << ",\"h\":" << myH
            << "}";
    }

    return ss.str();
}