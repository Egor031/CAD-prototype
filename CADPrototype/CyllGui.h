#pragma once

#include "CreateCyll.h"
#include "History.h"

struct GLFWwindow;

class CyllGui
{
public:
    CyllGui() = default;
    ~CyllGui() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(History& history, Document& doc);
    bool DoDraw();
    void EndFrame();

private:
    bool myInitialized = false;
   // CreateCyll CreateCyll;
};

