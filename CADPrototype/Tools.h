#pragma once

#include "PNG_Reader.h"
#include "BoxGui.h"
#include "CyllGui.h"

struct GLFWwindow;

class History;

class Tools
{
public:
    Tools() = default;
    ~Tools() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(History& history, Document& doc);
    void EndFrame();

private:
    bool myInitialized = false;
    PNG_Reader PNG_Reader;
    BoxGui BoxGui;
    CyllGui CyllGui;
};
