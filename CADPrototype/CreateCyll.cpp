#include "CreateCyll.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <sstream>
#include <cmath>

std::string CreateCyll::DrawCyll(char InX[100], char  InY[100], char  InZ[100], char Diam[100], char  Hei[100], char  axelX[100], char  axelY[100], char  axelZ[100], Document& doc)
{

    char* endptr;
    double intInX = std::stoi(InX);
    double  intInY = std::stoi(InY);
    double  intInZ = std::stoi(InZ);
    double  intDiam = std::stoi(Diam);
    double  intHei = std::stoi(Hei);
    double  intaxelX = std::stoi(axelX);
    double  intaxelY = std::stoi(axelY);
    double  intaxelZ = std::stoi(axelZ);

    gp_Pnt baseCenter(intInX, intInY, intInZ);
    gp_Pnt CordCenter(0, 0, 0);
    gp_Dir axisDir(intaxelX, intaxelY, intaxelZ);
    gp_Ax2 axis(CordCenter, axisDir);

    BRepPrimAPI_MakeCylinder makeCyl(axis, intDiam/2, intHei);
    TopoDS_Shape myCyll = makeCyl.Shape();
    doc.AddShape(myCyll);
    Handle(AIS_Shape) Display = new AIS_Shape(myCyll);
    std::string a;
    return (a);
}
