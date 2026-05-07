#include "CmdAddBox.h"

#include <sstream>

CmdAddBox::CmdAddBox(double dx, double dy, double dz)
    : myDx(dx), myDy(dy), myDz(dz)
{}

void CmdAddBox::Apply(Document& doc)
{
    if (myCreatedId == 0)
    {
        myCreatedId = doc.AddBox(myDx, myDy, myDz);
    }
    else
    {
        doc.AddBoxWithId(myCreatedId, myDx, myDy, myDz);
    }
}

void CmdAddBox::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CmdAddBox::ToJson() const
{
    std::ostringstream ss;

    if (myCreatedId != 0)
    {
        ss << "{"
            << "\"type\":\"AddBoxWithId\","
            << "\"id\":" << myCreatedId << ","
            << "\"dx\":" << myDx << ","
            << "\"dy\":" << myDy << ","
            << "\"dz\":" << myDz
            << "}";
    }
    else
    {        ss << "{"
            << "\"type\":\"AddBox\","
            << "\"dx\":" << myDx << ","
            << "\"dy\":" << myDy << ","
            << "\"dz\":" << myDz
            << "}";
    }

    return ss.str();
}