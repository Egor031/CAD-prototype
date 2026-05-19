/*#pragma once

#include <AIS_InteractiveContext.hxx>

#include "Document.h"

struct GLFWwindow;

class MakeFuse
{
public:
    MakeFuse() = default;
    ~MakeFuse() = default;

    std::string DrawFuse(TopoDS_Shape Shape1, TopoDS_Shape Shape2, Document& doc);

private:
    bool myInitialized = false;
};
*/


#pragma once

#include "ICommand.h"
#include "Document.h"
#include <memory>

class CreateFuse : public ICommand
{
public:
    CreateFuse(int id1, int id2);

    void Apply(Document& doc) override;
    void Undo(Document& doc) override;

    std::string Name() const override { return "CreateFuse"; }
    std::string ToJson() const override;

public:
    int Fid1, Fid2;
    EntityId myCreatedId = 0;
};
