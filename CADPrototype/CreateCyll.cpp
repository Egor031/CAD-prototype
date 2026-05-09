/*
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
    gp_Ax2 axis(baseCenter, axisDir);

    BRepPrimAPI_MakeCylinder makeCyl(axis, intDiam/2, intHei);
    TopoDS_Shape myCyll = makeCyl.Shape();
    doc.AddShape(myCyll);
    Handle(AIS_Shape) Display = new AIS_Shape(myCyll);
    std::string a;
    return (a);
}
*/

#include "CreateCyll.h"
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <sstream>
#include <memory>

CreateCyll::CreateCyll(double nullx, double nully, double nullz, double dia, double hei, double axx, double axy, double axz)
    : myDx(nullx), myDy(nully), myDz(nullz), Diametr(dia), Height(hei), AxisX(axx), AxisY(axy), AxisZ(axz)
{
}


void CreateCyll::Apply(Document& doc)
{
    if (myCreatedId != 0)
    {
        int id = myCreatedId;
        
        gp_Pnt baseCenter(myDx, myDy, myDz);
        //gp_Pnt CordCenter(0, 0, 0);
        gp_Dir axisDir(AxisX, AxisY, AxisZ);
        gp_Ax2 axis(baseCenter, axisDir);

        BRepPrimAPI_MakeCylinder makeCyl(axis, Diametr / 2, Height);
        TopoDS_Shape myCyll = makeCyl.Shape();

        double input[8] = { myDx, myDy, myDz, Diametr, Height, AxisX, AxisY, AxisZ };
        myCreatedId = id;

        doc.DrawShape("Cyll", myCyll, id, input);

    }
    else
    {
        int id = doc.GenerateId();
        

        gp_Pnt baseCenter(myDx, myDy, myDz);
        //gp_Pnt CordCenter(0, 0, 0);
        gp_Dir axisDir(AxisX, AxisY, AxisZ);
        gp_Ax2 axis(baseCenter, axisDir);

        BRepPrimAPI_MakeCylinder makeCyl(axis, Diametr / 2, Height);
        TopoDS_Shape myCyll = makeCyl.Shape();

        double input[8] = { myDx, myDy, myDz, Diametr, Height, AxisX, AxisY, AxisZ };
        myCreatedId = id;

        doc.DrawShape("Cyll", myCyll, id, input);
    }
}

void CreateCyll::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CreateCyll::ToJson() const
{
    std::ostringstream ss;

    if (myCreatedId != 0)
    {
        ss << "{"
            << "\"type\":\"DrawCyll\","
            << "\"id\":" << myCreatedId << ","
            << "\"dx\":" << myDx << ","
            << "\"dy\":" << myDy << ","
            << "\"dz\":" << myDz << ","
            << "\"Dia\":" << Diametr << ","
            << "\"Hei\":" << Height << ","
            << "\"AxX\":" << AxisX << ","
            << "\"AxY\":" << AxisY << ","
            << "\"AxZ\":" << AxisZ << ","
            << "}";
    }

    return ss.str();
}
