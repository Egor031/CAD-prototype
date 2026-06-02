#pragma once

#include "Cut.h"
#include "History.h"

struct GLFWwindow;

class CutGui
{
public:
    CutGui() = default;
    ~CutGui() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(History& history, Document& doc);
    bool DoDraw();
    void EndFrame();

private:
    bool myInitialized = false;
};

