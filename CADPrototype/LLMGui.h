#pragma once

#include "History.h"

struct GLFWwindow;

class LLMGui
{
public:
    LLMGui() = default;
    ~LLMGui() = default;

    void Init(GLFWwindow* theWindow);
    void Shutdown();

    void BeginFrame();
    void Draw(History& history, Document& doc);
    bool DoDraw();
    void EndFrame();

private:
    bool myInitialized = false;
    std::string myEndpoint = "http://localhost:1234";

    std::string myModel = ""; // позже подставим из /v1/models, пока можно руками

    std::string myHistoryJson;

    std::string myStateJson;

    std::string myLlmResponse;
    std::string myLlmRequest;
    std::string myLlmRaw;
    std::string myLlmError;
};

