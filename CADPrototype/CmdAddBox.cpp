#include "CmdAddBox.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <sstream>

CmdAddBox::CmdAddBox(double dx, double dy, double dz)
    : myDx(dx), myDy(dy), myDz(dz)
{
}

void CmdAddBox::Apply(Document& doc)
{
    myCreatedId = doc.AddBox(myDx, myDy, myDz);
}

void CmdAddBox::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CmdAddBox::ToJson() const
{
    std::ostringstream ss;
    ss << "{"
        << "\"type\":\"AddBox\","
        << "\"dx\":" << myDx << ","
        << "\"dy\":" << myDy << ","
        << "\"dz\":" << myDz
        << "}";
    return ss.str();
}
