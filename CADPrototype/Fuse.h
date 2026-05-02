#pragma once

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
