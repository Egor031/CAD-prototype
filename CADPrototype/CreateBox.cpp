#include "CreateBox.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <sstream>

void CreateBox::DrawBox(char InX[100], char  InY[100], char  InZ[100], char Len[100], char  Wid[100], char  Hei[100])
{
    for(int i = 0;i < 100;i++)
    {
        if ((isdigit(InX[i])&&(InX!=NULL)))
            if ((isdigit(InX[i]) && (InX != NULL)))
                if (isdigit(InZ[i]))
                    if (isdigit(Len[i]))
                        if (isdigit(Wid[i]))
                            if (isdigit(Len[i]))
                                ;
                            else return;
                        else return;
                    else return;
                else return;
            else return;
        else return;
    }
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
    //doc.AddShape(myBox);
    //Handle(AIS_Shape) Display = new AIS_Shape(myBox);
    //myContext->Display(Display, false);
    //GlfwOcctView.DsShapeToView(myBox);
}
