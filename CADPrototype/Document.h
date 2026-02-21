#pragma once

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>

#include <vector>
#include <cstdint>

using EntityId = uint64_t;

class Document
{
public:
    explicit Document(const Handle(AIS_InteractiveContext)& ctx);

    EntityId AddShape(const TopoDS_Shape& shape);

    // Удаление: вернём bool, чтобы команда могла понимать, получилось ли
    bool RemoveShape(EntityId id);

    Handle(AIS_Shape) GetShape(EntityId id);

    // Для DeleteEntity Undo
    bool TryGetTopoShape(EntityId id, TopoDS_Shape& outShape) const;
    bool AddShapeWithId(EntityId id, const TopoDS_Shape& shape);

    // Для UI/отладки
    std::vector<EntityId> ListIds() const;

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
    std::string ExportStateJson() const;
};

struct ShapeEntry
{
    EntityId id;
    Handle(AIS_Shape) ais;

    // metadata for LLM/state
    std::string kind;   // "Box"
    double dx = 0, dy = 0, dz = 0;
};