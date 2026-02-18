#pragma once

struct GLFWwindow;

class GuiApp
{
public:
    GuiApp() = default;
    ~GuiApp() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw();      // тут рисуем окна/панели
    void EndFrame();

private:
    bool myInitialized = false;
};
