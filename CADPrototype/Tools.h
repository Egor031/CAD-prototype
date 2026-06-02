#pragma once

#include "PNG_Reader.h"
#include "BoxGui.h"
#include "CyllGui.h"
#include "FuseGui.h"
#include "CutGui.h"
#include "LLMGui.h"
#include "History.h"

struct GLFWwindow;

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
    void IniPNG();

private:
    bool myInitialized = false;
    PNG_Reader PNG_Reader;
    BoxGui BoxGui;
    CyllGui CyllGui;
    FuseGui FuseGui;
    CutGui CutGui;
    LLMGui LLMGui;
};
