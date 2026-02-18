#pragma once

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>

#include <vector>

using EntityId = uint64_t;

class Document
{
public:
    Document(const Handle(AIS_InteractiveContext)& ctx);

    EntityId AddShape(const TopoDS_Shape& shape);
    void RemoveShape(EntityId id);

    Handle(AIS_Shape) GetShape(EntityId id);

private:
    EntityId GenerateId();

private:
    Handle(AIS_InteractiveContext) myContext;

    struct ShapeEntry
    {
        EntityId id;
        Handle(AIS_Shape) ais;
    };

    std::vector<ShapeEntry> myShapes;
    EntityId myNextId = 1;
};
