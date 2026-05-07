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
    
    
    std::string a;

    std::ostringstream ss;
    /*
    std::string myBoxKind = "Box";
    int myCreatedId = doc.GenerateId();
        ss << "{"
            << "\"type\":\"CreateBox\","
            << "\"id\":" << myCreatedId << ","
            << "\"sx\":" << intInX << ","
            << "\"sy\":" << intInY << ","
            << "\"sz\":" << intInZ << ","
            << "\"len\":" << intLen << ","
            << "\"wid\":" << intWid << ","
            << "\"hei\":" << intHei
            << "}";
            */
        doc.AddShape(myBox);
      //  doc.DrawShape(myBoxKind, myBox, ss.str());
    return a;

}
