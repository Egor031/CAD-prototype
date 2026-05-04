#include "CmdAddCircle.h"

#include <sstream>

CmdAddCircle::CmdAddCircle(double cx, double cy, double r)
    : myCx(cx), myCy(cy), myR(r)
{}

CmdAddCircle::CmdAddCircle(EntityId id, double cx, double cy, double r)
    : myCreatedId(id), myCx(cx), myCy(cy), myR(r)
{}

void CmdAddCircle::Apply(Document& doc)
{
    if (myCreatedId == 0)
    {
        myCreatedId = doc.AddCircle(myCx, myCy, myR);
    }
    else
    {
        doc.AddCircleWithId(myCreatedId, myCx, myCy, myR);
    }
}

void CmdAddCircle::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CmdAddCircle::ToJson() const
{
    std::ostringstream ss;

    if (myCreatedId != 0)
    {
        ss << "{"
            << "\"type\":\"AddCircleWithId\","
            << "\"id\":" << myCreatedId << ","
            << "\"cx\":" << myCx << ","
            << "\"cy\":" << myCy << ","
            << "\"r\":" << myR
            << "}";
    }
    else
    {
        ss << "{"
            << "\"type\":\"AddCircle\","
            << "\"cx\":" << myCx << ","
            << "\"cy\":" << myCy << ","
            << "\"r\":" << myR
            << "}";
    }

    return ss.str();
}