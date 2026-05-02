#pragma once

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>

#include <cstdint>
#include <string>
#include <vector>

using EntityId = uint64_t;

class Document
{
public:
    explicit Document(const Handle(AIS_InteractiveContext)& ctx);

    // Generic shape API (на будущее для напарника)
    EntityId AddShape(const TopoDS_Shape& shape);
    bool AddShapeWithId(EntityId id, const TopoDS_Shape& shape);

    // CAD-level API (то, что нужно LLM/эскизы/параметрика)
    EntityId AddBox(double dx, double dy, double dz);
    bool AddBoxWithId(EntityId id, double dx, double dy, double dz);

    // 2D vector API
    EntityId AddLine(double x1, double y1, double x2, double y2);
    bool AddLineWithId(EntityId id, double x1, double y1, double x2, double y2);

    bool TryGetSelectedEntityId(EntityId& outId) const;

    bool TryGetBoxParams(EntityId id, double& dx, double& dy, double& dz) const;
    bool UpdateBox(EntityId id, double dx, double dy, double dz);

    //Extract seleted TopoSD_Shape from context
    TopoDS_Shape GetSelectedTPDSShape();

    void TempDell();

    bool RemoveShape(EntityId id);
    Handle(AIS_Shape) GetShape(EntityId id);

    // Для DeleteEntity Undo
    bool TryGetTopoShape(EntityId id, TopoDS_Shape& outShape) const;

    // Для корректного Undo после удаления (восстановить метаданные)
    bool TryGetMeta(EntityId id, std::string& kind, double& dx, double& dy, double& dz) const;
    bool AddShapeWithIdAndMeta(EntityId id,
        const TopoDS_Shape& shape,
        const std::string& kind,
        double dx, double dy, double dz);

    // Для UI/отладки
    std::vector<EntityId> ListIds() const;

    // Снимок текущего состояния (то, что даём LLM)
    std::string ExportStateJson() const;

    // (опционально) полная очистка сцены
    void Clear();

private:
    EntityId GenerateId();

private:
    Handle(AIS_InteractiveContext) myContext;

    struct ShapeEntry
    {
        EntityId id = 0;
        Handle(AIS_Shape) ais;

        std::string kind;   // "Box" / "Line" / "Generic"

        // Box params
        double dx = 0, dy = 0, dz = 0;

        // Line params
        double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    };

    std::vector<ShapeEntry> myShapes;
    EntityId myNextId = 1;
};