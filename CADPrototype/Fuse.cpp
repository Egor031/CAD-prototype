/*#include "Fuse.h"

#include <AIS_Shape.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <sstream>
#include <cmath>

std::string MakeFuse::DrawFuse(TopoDS_Shape Shape1, TopoDS_Shape Shape2, Document& doc)
{

  
    BRepAlgoAPI_Fuse fuse(Shape1, Shape2);
    fuse.Build();
    TopoDS_Shape myCyll = fuse.Shape();
    doc.AddShape(myCyll);
    std::string a;
    return (a);
}

*/

#include "Fuse.h"
#include <AIS_Shape.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <sstream>
#include <memory>

CreateFuse::CreateFuse(int id1, int id2)
    : Fid1(id1),Fid2(id2)
{
}


void CreateFuse::Apply(Document& doc)
{
    if (myCreatedId != 0)
    {
        int id = myCreatedId;

        TopoDS_Shape Shape1 = doc.GetTopoDSfromID(Fid1);
        TopoDS_Shape Shape2 = doc.GetTopoDSfromID(Fid2);

        BRepAlgoAPI_Fuse fuse(Shape1, Shape2);
        fuse.Build();
        TopoDS_Shape myCyll = fuse.Shape();

        double input[2] = { Fid1,Fid2 };

        doc.DrawShape("Fuse", myCyll, id, input);

    }
    else
    {
        int id = doc.GenerateId();
        myCreatedId = id;

        TopoDS_Shape Shape1 = doc.GetTopoDSfromID(Fid1);
        TopoDS_Shape Shape2 = doc.GetTopoDSfromID(Fid2);
        doc.RemoveShapeWithoutJSON(Fid1);
        doc.RemoveShapeWithoutJSON(Fid2);

        BRepAlgoAPI_Fuse fuse(Shape1, Shape2);
        fuse.Build();
        TopoDS_Shape myCyll = fuse.Shape();

        double input[2] = { Fid1,Fid2 };

        doc.DrawShape("Fuse", myCyll, id, input);
    }
}

void CreateFuse::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CreateFuse::ToJson() const
{
    std::ostringstream ss;

    if (myCreatedId != 0)
    {
        ss << "{"
            << "\"type\":\"DrawFuse\","
            << "\"id\":" << myCreatedId << ","
            << "\"Fid1\":" << Fid1 << ","
            << "\"Fid2\":" << Fid2 << ","
            << "}";
    }

    return ss.str();
}
