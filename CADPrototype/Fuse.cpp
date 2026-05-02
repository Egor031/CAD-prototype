#include "Fuse.h"

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
