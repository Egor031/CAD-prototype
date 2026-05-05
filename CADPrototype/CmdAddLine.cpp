#include "CmdAddLine.h"

#include <sstream>

CmdAddLine::CmdAddLine(double x1, double y1, double x2, double y2)
    : myX1(x1), myY1(y1), myX2(x2), myY2(y2)
{}

CmdAddLine::CmdAddLine(EntityId id, double x1, double y1, double x2, double y2)
    : myId(id), myX1(x1), myY1(y1), myX2(x2), myY2(y2)
{}

void CmdAddLine::Apply(Document& doc)
{
    if (myId == 0)
        myId = doc.AddLine(myX1, myY1, myX2, myY2);
    else
        doc.AddLineWithId(myId, myX1, myY1, myX2, myY2);
}

void CmdAddLine::Undo(Document& doc)
{
    if (myId != 0)
        doc.RemoveShape(myId);
}

std::string CmdAddLine::Name() const
{
    return "AddLine";
}

std::string CmdAddLine::ToJson() const
{
    std::ostringstream ss;

    if (myId != 0)
    {
        ss << "{\"type\":\"AddLineWithId\""
            << ",\"id\":" << myId
            << ",\"x1\":" << myX1
            << ",\"y1\":" << myY1
            << ",\"x2\":" << myX2
            << ",\"y2\":" << myY2
            << "}";
    }
    else
    {
        ss << "{\"type\":\"AddLine\""
            << ",\"x1\":" << myX1
            << ",\"y1\":" << myY1
            << ",\"x2\":" << myX2
            << ",\"y2\":" << myY2
            << "}";
    }

    return ss.str();
}