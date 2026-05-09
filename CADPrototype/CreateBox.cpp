/*#include "CreateBox.h"

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
            
        doc.AddShape(myBox);
      //  doc.DrawShape(myBoxKind, myBox, ss.str());
    return a;

}

*/

#include "CreateBox.h"
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <sstream>
#include <memory>

CreateBox::CreateBox(double nullx, double nully, double nullz, double len, double wid, double hei)
    : myDx(nullx), myDy(nully), myDz(nullz), Lenght(len), Width(wid), Height(hei)
{
}


void CreateBox::Apply(Document& doc)
{
    if (myCreatedId != 0)
    {
        int id = myCreatedId;

        gp_Pnt p1(myDx, myDy, myDz);
        gp_Pnt p2(myDx + Lenght, myDy + Width, myDz + Height);

        BRepPrimAPI_MakeBox makeBox(p1, p2);
        TopoDS_Shape myBox = makeBox.Shape();

        double input[6] = { myDx, myDy, myDz, Lenght, Width, Height };

        myCreatedId = id;

        doc.DrawShape("Box", myBox, id, input);

    }
    else
    {
        int id = doc.GenerateId();

        gp_Pnt p1(myDx, myDy, myDz);
        gp_Pnt p2(myDx + Lenght, myDy + Width, myDz + Height);

        BRepPrimAPI_MakeBox makeBox(p1, p2);
        TopoDS_Shape myBox = makeBox.Shape();

        double input[6] = { myDx, myDy, myDz, Lenght, Width, Height };

        myCreatedId = id;

        doc.DrawShape("Box", myBox, id, input);
    }
}

void CreateBox::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CreateBox::ToJson() const
{
    std::ostringstream ss;

    if (myCreatedId != 0)
    {
        ss << "{"
            << "\"type\":\"DrawBox\","
            << "\"id\":" << myCreatedId << ","
            << "\"dx\":" << myDx << ","
            << "\"dy\":" << myDy << ","
            << "\"dz\":" << myDz << ","
            << "\"Len\":" << Lenght << ","
            << "\"Wid\":" << Width << ","
            << "\"Hei\":" << Height << ","
            << "}";
    }

    return ss.str();
}
