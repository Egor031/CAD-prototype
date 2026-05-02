#pragma once

#include "PNG_Reader.h"
#include "BoxGui.h"
#include "CyllGui.h"
#include "FuseGui.h"

struct GLFWwindow;

class Tools
{
public:
    Tools() = default;
    ~Tools() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(Document& doc);     
    void EndFrame();

private:
    bool myInitialized = false;
    PNG_Reader PNG_Reader;
    BoxGui BoxGui;
    CyllGui CyllGui;
    FuseGui FuseGui;
};
