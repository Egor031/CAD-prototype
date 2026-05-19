#pragma once

#include "Fuse.h"
#include "History.h"

struct GLFWwindow;

class FuseGui
{
public:
    FuseGui() = default;
    ~FuseGui() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(History& history, Document& doc);
    bool DoDraw();
    void EndFrame();

private:
    bool myInitialized = false;
};

