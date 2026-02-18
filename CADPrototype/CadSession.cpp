#include "CadSession.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>

CadSession::CadSession(const Handle(AIS_InteractiveContext)& theCtx)
    : myContext(theCtx)
{
}

void CadSession::AddBox(double dx, double dy, double dz)
{
    TopoDS_Shape shape = BRepPrimAPI_MakeBox(dx, dy, dz).Shape();
    Handle(AIS_Shape) ais = new AIS_Shape(shape);

    myContext->Display(ais, AIS_Shaded, 0, Standard_True);
    myShapes.push_back(ais);
}


void CadSession::AddCone(double r1, double r2, double h)
{
    TopoDS_Shape shape = BRepPrimAPI_MakeCone(r1, r2, h).Shape();
    Handle(AIS_Shape) ais = new AIS_Shape(shape);

    myContext->Display(ais, AIS_Shaded, 0, Standard_True);
    myShapes.push_back(ais);
}
