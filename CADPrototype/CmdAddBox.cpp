#include "CmdAddBox.h"

#include <BRepPrimAPI_MakeBox.hxx>

CmdAddBox::CmdAddBox(double dx, double dy, double dz)
    : myDx(dx), myDy(dy), myDz(dz)
{
}

void CmdAddBox::Apply(Document& doc)
{
    TopoDS_Shape shape = BRepPrimAPI_MakeBox(myDx, myDy, myDz).Shape();
    myCreatedId = doc.AddShape(shape);
    std::cout << "AddBox created id=" << myCreatedId << std::endl;

}

void CmdAddBox::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}
