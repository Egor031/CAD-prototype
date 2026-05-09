#pragma once

#include "CreateBox.h"
#include "History.h"

#include <memory>

struct GLFWwindow;

class BoxGui
{
public:
    BoxGui() = default;
    ~BoxGui() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(History& history, Document& doc);
    bool DoDraw();
    void EndFrame();

private:
    bool myInitialized = false;
    //CreateBox CreateBox;
};

