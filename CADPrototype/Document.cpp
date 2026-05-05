#include "Document.h"

#include <sstream>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Pnt.hxx>
#include <AIS_InteractiveObject.hxx>
#include <gp_Circ.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <iostream>

Document::Document(const Handle(AIS_InteractiveContext)& ctx)
    : myContext(ctx)
{
}

EntityId Document::GenerateId()
{
    return myNextId++;
}

EntityId Document::AddShape(const TopoDS_Shape& shape)
{
    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_Shaded, 0, Standard_True);

    EntityId id = GenerateId();
    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Generic";
    myShapes.push_back(e);

    return id;
}


TopoDS_Shape Document::GetSelectedTPDSShape()
{
    myContext->InitSelected();
    Handle(AIS_InteractiveObject) ais = myContext->SelectedInteractive();
    Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(ais);
    TopoDS_Shape shape = aisShape->Shape();
    return(shape);
}

void Document::TempDell()
{
    static int i = myShapes.size();
    //TopoDS_Shape shape = 
    myContext->InitSelected();
    Handle(AIS_InteractiveObject) ais = myContext->SelectedInteractive();
    myContext->Remove(ais, Standard_True);
    //AddShape(shape);
}

bool Document::RemoveShape(EntityId id)
{
    for (auto it = myShapes.begin(); it != myShapes.end(); ++it)
    {
        if (it->id == id)
        {
            myContext->Remove(it->ais, Standard_True);
            myShapes.erase(it);
            return true;
        }
    }

    std::cout << "RemoveShape id=" << id << " shapes=" << myShapes.size() << std::endl;
    return false;
}



Handle(AIS_Shape) Document::GetShape(EntityId id)
{
    for (auto& s : myShapes)
        if (s.id == id)
            return s.ais;

    return nullptr;
}

bool Document::TryGetTopoShape(EntityId id, TopoDS_Shape& outShape) const
{
    for (const auto& s : myShapes)
    {
        if (s.id == id && !s.ais.IsNull())
        {
            outShape = s.ais->Shape();
            return true;
        }
    }
    return false;
}

bool Document::AddShapeWithId(EntityId id, const TopoDS_Shape& shape)
{
    for (const auto& s : myShapes)
        if (s.id == id)
            return false;

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_Shaded, 0, Standard_True);

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Generic";
    myShapes.push_back(e);

    if (id >= myNextId)
        myNextId = id + 1;

    return true;
}

std::vector<EntityId> Document::ListIds() const
{
    std::vector<EntityId> ids;
    ids.reserve(myShapes.size());
    for (const auto& s : myShapes)
        ids.push_back(s.id);
    return ids;
}

EntityId Document::AddBox(double dx, double dy, double dz)
{
    TopoDS_Shape shape = BRepPrimAPI_MakeBox(dx, dy, dz).Shape();

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_Shaded, 0, Standard_True);

    EntityId id = GenerateId();

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Box";
    e.dx = dx; e.dy = dy; e.dz = dz;
    myShapes.push_back(e);

    return id;
}

bool Document::AddBoxWithId(EntityId id, double dx, double dy, double dz)
{
    for (const auto& s : myShapes)
        if (s.id == id)
            return false;

    TopoDS_Shape shape = BRepPrimAPI_MakeBox(dx, dy, dz).Shape();
    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_Shaded, 0, Standard_True);

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Box";
    e.dx = dx; e.dy = dy; e.dz = dz;
    myShapes.push_back(e);

    if (id >= myNextId)
        myNextId = id + 1;

    return true;
}

std::string Document::ExportStateJson() const
{
    std::ostringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < myShapes.size(); ++i)
    {
        const auto& e = myShapes[i];
        ss << "  {\"id\":" << (unsigned long long)e.id
            << ",\"kind\":\"" << e.kind << "\"";

        if (e.kind == "Box")
        {
            ss << ",\"dx\":" << e.dx
                << ",\"dy\":" << e.dy
                << ",\"dz\":" << e.dz;
        }
        else if (e.kind == "Line")
        {
            ss << ",\"x1\":" << e.x1
                << ",\"y1\":" << e.y1
                << ",\"x2\":" << e.x2
                << ",\"y2\":" << e.y2;
        }
        else if (e.kind == "Circle")
        {
            ss << ",\"cx\":" << e.cx
                << ",\"cy\":" << e.cy
                << ",\"r\":" << e.r;
        }
        else if (e.kind == "Rectangle")
        {
            ss << ",\"x\":" << e.rectX
                << ",\"y\":" << e.rectY
                << ",\"w\":" << e.rectW
                << ",\"h\":" << e.rectH;
        }

        ss << "}";
        if (i + 1 < myShapes.size()) ss << ",";
        ss << "\n";
    }
    ss << "]";
    return ss.str();
}

void Document::Clear()
{
    for (auto& e : myShapes)
    {
        if (!e.ais.IsNull())
            myContext->Remove(e.ais, Standard_True);
    }
    myShapes.clear();
}

bool Document::TryGetBoxParams(EntityId id, double& dx, double& dy, double& dz) const
{
    for (const auto& e : myShapes)
    {
        if (e.id == id && e.kind == "Box")
        {
            dx = e.dx; dy = e.dy; dz = e.dz;
            return true;
        }
    }
    return false;
}

bool Document::UpdateBox(EntityId id, double dx, double dy, double dz)
{
    for (auto& e : myShapes)
    {
        if (e.id != id)
            continue;

        if (e.kind != "Box")
            return false;

        TopoDS_Shape newShape = BRepPrimAPI_MakeBox(dx, dy, dz).Shape();

        // Способ 1 (надёжный и простой): удалить+создать новый AIS_Shape
        if (!e.ais.IsNull())
            myContext->Remove(e.ais, Standard_True);

        e.ais = new AIS_Shape(newShape);
        myContext->Display(e.ais, AIS_Shaded, 0, Standard_True);

        e.dx = dx; e.dy = dy; e.dz = dz;
        return true;
    }
    return false;
}

bool Document::TryGetMeta(EntityId id, std::string& kind, double& dx, double& dy, double& dz) const
{
    for (const auto& e : myShapes)
    {
        if (e.id == id)
        {
            kind = e.kind;
            dx = e.dx; dy = e.dy; dz = e.dz;
            return true;
        }
    }
    return false;
}

bool Document::AddShapeWithIdAndMeta(EntityId id,
    const TopoDS_Shape& shape,
    const std::string& kind,
    double dx, double dy, double dz)
{
    for (const auto& s : myShapes)
        if (s.id == id)
            return false;

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_Shaded, 0, Standard_True);

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = kind;
    e.dx = dx; e.dy = dy; e.dz = dz;
    myShapes.push_back(e);

    if (id >= myNextId)
        myNextId = id + 1;

    return true;
}

EntityId Document::AddLine(double x1, double y1, double x2, double y2)
{
    TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(
        gp_Pnt(x1, y1, 0.0),
        gp_Pnt(x2, y2, 0.0)
    ).Shape();

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_WireFrame, 0, Standard_True);

    EntityId id = GenerateId();

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Line";
    e.x1 = x1;
    e.y1 = y1;
    e.x2 = x2;
    e.y2 = y2;

    myShapes.push_back(e);

    return id;
}

bool Document::AddLineWithId(EntityId id, double x1, double y1, double x2, double y2)
{
    for (const auto& s : myShapes)
        if (s.id == id)
            return false;

    TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(
        gp_Pnt(x1, y1, 0.0),
        gp_Pnt(x2, y2, 0.0)
    ).Shape();

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_WireFrame, 0, Standard_True);

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Line";
    e.x1 = x1;
    e.y1 = y1;
    e.x2 = x2;
    e.y2 = y2;

    myShapes.push_back(e);

    if (id >= myNextId)
        myNextId = id + 1;

    return true;
}

bool Document::TryGetSelectedEntityId(EntityId& outId) const
{
    outId = 0;

    if (myContext.IsNull())
        return false;

    myContext->InitSelected();

    if (!myContext->MoreSelected())
        return false;

    Handle(AIS_InteractiveObject) selected = myContext->SelectedInteractive();

    if (selected.IsNull())
        return false;

    for (const auto& e : myShapes)
    {
        if (e.ais == selected)
        {
            outId = e.id;
            return true;
        }
    }

    return false;
}

EntityId Document::AddCircle(double cx, double cy, double r)
{
    gp_Circ circle(
        gp_Ax2(gp_Pnt(cx, cy, 0.0), gp_Dir(0.0, 0.0, 1.0)),
        r
    );

    TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(circle).Shape();

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_WireFrame, 0, Standard_True);

    EntityId id = GenerateId();

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Circle";
    e.cx = cx;
    e.cy = cy;
    e.r = r;

    myShapes.push_back(e);

    return id;
}

bool Document::AddCircleWithId(EntityId id, double cx, double cy, double r)
{
    for (const auto& s : myShapes)
        if (s.id == id)
            return false;

    gp_Circ circle(
        gp_Ax2(gp_Pnt(cx, cy, 0.0), gp_Dir(0.0, 0.0, 1.0)),
        r
    );

    TopoDS_Shape shape = BRepBuilderAPI_MakeEdge(circle).Shape();

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_WireFrame, 0, Standard_True);

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Circle";
    e.cx = cx;
    e.cy = cy;
    e.r = r;

    myShapes.push_back(e);

    if (id >= myNextId)
        myNextId = id + 1;

    return true;
}

EntityId Document::AddRectangle(double x, double y, double w, double h)
{
    BRepBuilderAPI_MakePolygon poly;
    poly.Add(gp_Pnt(x, y, 0.0));
    poly.Add(gp_Pnt(x + w, y, 0.0));
    poly.Add(gp_Pnt(x + w, y + h, 0.0));
    poly.Add(gp_Pnt(x, y + h, 0.0));
    poly.Close();

    TopoDS_Shape shape = poly.Shape();

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_WireFrame, 0, Standard_True);

    EntityId id = GenerateId();

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Rectangle";
    e.rectX = x;
    e.rectY = y;
    e.rectW = w;
    e.rectH = h;

    myShapes.push_back(e);

    return id;
}

bool Document::AddRectangleWithId(EntityId id, double x, double y, double w, double h)
{
    for (const auto& s : myShapes)
        if (s.id == id)
            return false;

    BRepBuilderAPI_MakePolygon poly;
    poly.Add(gp_Pnt(x, y, 0.0));
    poly.Add(gp_Pnt(x + w, y, 0.0));
    poly.Add(gp_Pnt(x + w, y + h, 0.0));
    poly.Add(gp_Pnt(x, y + h, 0.0));
    poly.Close();

    TopoDS_Shape shape = poly.Shape();

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_WireFrame, 0, Standard_True);

    ShapeEntry e;
    e.id = id;
    e.ais = ais;
    e.kind = "Rectangle";
    e.rectX = x;
    e.rectY = y;
    e.rectW = w;
    e.rectH = h;

    myShapes.push_back(e);

    if (id >= myNextId)
        myNextId = id + 1;

    return true;
}