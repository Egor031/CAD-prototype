#include "Cut.h"
#include <AIS_Shape.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <sstream>
#include <memory>

CreateCut::CreateCut(int id1, int id2)
    : Fid1(id1), Fid2(id2)
{
}


void CreateCut::Apply(Document& doc)
{
    if (myCreatedId != 0)
    {
        int id = myCreatedId;

        TopoDS_Shape Shape1 = doc.GetTopoDSfromID(Fid1);
        TopoDS_Shape Shape2 = doc.GetTopoDSfromID(Fid2);

        BRepAlgoAPI_Cut fuse(Shape1, Shape2);
        fuse.Build();
        TopoDS_Shape myCyll = fuse.Shape();

        double input[2] = { Fid1,Fid2 };

        doc.DrawShape("Cut", myCyll, id, input);

    }
    else
    {
        int id = doc.GenerateId();
        myCreatedId = id;

        TopoDS_Shape Shape1 = doc.GetTopoDSfromID(Fid1);
        TopoDS_Shape Shape2 = doc.GetTopoDSfromID(Fid2);
        doc.RemoveShapeWithoutJSON(Fid1);
        doc.RemoveShapeWithoutJSON(Fid2);

        BRepAlgoAPI_Cut fuse(Shape1, Shape2);
        fuse.Build();
        TopoDS_Shape myCyll = fuse.Shape();

        double input[2] = { Fid1,Fid2 };

        doc.DrawShape("Cut", myCyll, id, input);
    }
}

void CreateCut::Undo(Document& doc)
{
    if (myCreatedId != 0)
        doc.RemoveShape(myCreatedId);
}

std::string CreateCut::ToJson() const
{
    std::ostringstream ss;

    if (myCreatedId != 0)
    {
        ss << "{"
            << "\"type\":\"DrawCut\","
            << "\"id\":" << myCreatedId << ","
            << "\"Fid1\":" << Fid1 << ","
            << "\"Fid2\":" << Fid2 << ","
            << "}";
    }

    return ss.str();
}
