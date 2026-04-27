#pragma once

#include "PNG_Reader.h"

struct GLFWwindow;

class Tools
{
public:
    Tools() = default;
    ~Tools() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw();      // тут рисуем окна/панели
    void EndFrame();

private:
    bool myInitialized = false;
    PNG_Reader PNG_Reader;
};
