#include "Document.h"

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
    myShapes.push_back({ id, ais });
    return id;
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
    // не добавляем, если id уже существует
    for (const auto& s : myShapes)
        if (s.id == id)
            return false;

    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    myContext->Display(ais, AIS_Shaded, 0, Standard_True);

    myShapes.push_back({ id, ais });

    // поддерживаем генератор id, чтобы не было коллизий
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
