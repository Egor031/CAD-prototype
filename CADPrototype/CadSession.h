#pragma once

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <vector>

class CadSession
{
public:
    CadSession(const Handle(AIS_InteractiveContext)& theCtx);

    void AddBox(double dx, double dy, double dz);
    void AddCone(double r1, double r2, double h);

private:
    Handle(AIS_InteractiveContext) myContext;
    std::vector<Handle(AIS_Shape)> myShapes;
};
