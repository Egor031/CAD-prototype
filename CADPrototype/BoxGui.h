#pragma once

#include "CreateBox.h"

struct GLFWwindow;

class BoxGui
{
public:
    BoxGui() = default;
    ~BoxGui() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw();      // тут рисуем окна/панели
    void EndFrame();

private:
    bool myInitialized = false;
    CreateBox CreateBox;
};

