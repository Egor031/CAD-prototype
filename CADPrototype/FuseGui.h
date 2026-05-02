#pragma once

#include "Fuse.h"

struct GLFWwindow;

class FuseGui
{
public:
    FuseGui() = default;
    ~FuseGui() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(Document& doc);
    bool DoDraw();
    void EndFrame();

private:
    bool myInitialized = false;
    MakeFuse MakeFuse;
};

