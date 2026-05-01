/*
#include "CreateBox.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <sstream>

std::string CreateBox::DrawBox(char InX[100], char  InY[100], char  InZ[100], char Len[100], char  Wid[100], char  Hei[100], Document& doc)
{
   
    char* endptr;
    double intInX = std::stoi(InX);
    double intInY = std::stoi(InY);
    double intInZ = std::stoi(InZ);
    double intLen = std::stoi(Len);
    double intWid = std::stoi(Wid);
    double intHei = std::stoi(Hei);
    
    gp_Pnt p1(intInX, intInY, intInZ);  
    gp_Pnt p2(intInX+intLen, intInY+intWid, intInZ+intHei); 

    BRepPrimAPI_MakeBox makeBox(p1, p2);
    TopoDS_Shape myBox = makeBox.Shape();
    doc.AddShape(myBox);
    std::ostringstream a;
    a << "{"
        << "\"type\":\"CreateBox\","
        << "\"0x\":" << intInX << ","
        << "\"0y\":" << intInY << ","
        << "\"0z\":" << intInZ << ","
        << "\"len\":" << intLen << ","
        << "\"wid\":" << intWid << ","
        << "\"hei\":" << intHei
        << "}";
   // History::GetJSON;

    return a.str();
}


*/



#include "CreateBox.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <sstream>

CreateBox::CreateBox(double nx, double ny, double nz, double len, double wid, double hei)
    : NullX (nx), NullY (ny), NullZ (nz), Lenght (len), Width (wid), Height (hei)
{
}

void CreateBox::Apply(Document& doc)
{
    gp_Pnt p1(NullX, NullY, NullZ);
    gp_Pnt p2(NullX + Lenght, NullY + Width, NullZ + Height);

    BRepPrimAPI_MakeBox makeBox(p1, p2);
    TopoDS_Shape myBox = makeBox.Shape();
    doc.AddShape(myBox);
}

void CreateBox::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CreateBox::ToJson() const
{
    std::ostringstream ss;
    ss << "{"
        << "\"type\":\"CreateBox\","
        << "\"0x\":" << NullX << ","
        << "\"0y\":" << NullX << ","
        << "\"0z\":" << NullX << ","
        << "\"len\":" << Lenght << ","
        << "\"wid\":" << Width << ","
        << "\"hei\":" << Height
        << "}";
    return ss.str();
}
