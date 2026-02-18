#include "Document.h"

#include <TopoDS_Shape.hxx>

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

void Document::RemoveShape(EntityId id)
{
    for (auto it = myShapes.begin(); it != myShapes.end(); ++it)
    {
        if (it->id == id)
        {
            myContext->Remove(it->ais, Standard_True);
            myShapes.erase(it);
            return;
        }
    }
    std::cout << "RemoveShape id=" << id << " shapes=" << myShapes.size() << std::endl;

}

Handle(AIS_Shape) Document::GetShape(EntityId id)
{
    for (auto& s : myShapes)
        if (s.id == id)
            return s.ais;

    return nullptr;
}
