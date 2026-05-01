#include "CreateBox.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <sstream>

std::string CreateBox::DrawBox(char InX[100], char  InY[100], char  InZ[100], char Len[100], char  Wid[100], char  Hei[100], Document& doc)
{
   
    char* endptr;
    int intInX = std::stoi(InX);
    int intInY = std::stoi(InY);
    int intInZ = std::stoi(InZ);
    int intLen = std::stoi(Len);
    int intWid = std::stoi(Wid);
    int intHei = std::stoi(Hei);
    
    gp_Pnt p1(intInX, intInY, intInZ);  
    gp_Pnt p2(intInX+intLen, intInY+intWid, intInZ+intHei); 

    BRepPrimAPI_MakeBox makeBox(p1, p2);
    TopoDS_Shape myBox = makeBox.Shape();
    doc.AddShape(myBox);
    std::string a;
    return (a);
}
