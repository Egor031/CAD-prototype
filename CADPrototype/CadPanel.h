#pragma once

#include <string>

class History;
class Document;

class CadPanel
{
public:
    // Возвращает true, если модель была изменена (чтобы снаружи сделать UpdateCurrentViewer)
    bool Draw(History& history, Document& doc);

private:
    std::string myHistoryJson;

    std::string myStateJson;

    bool myShowDebug = false;

    std::string myImportJson =
        "[\n"
        "  {\"type\":\"AddRectangle\",\"x\":0,\"y\":0,\"w\":100,\"h\":60},\n"
        "  {\"type\":\"AddCircle\",\"cx\":50,\"cy\":30,\"r\":20},\n"
        "  {\"type\":\"AddLine\",\"x1\":0,\"y1\":30,\"x2\":100,\"y2\":30}\n"
        "]";

    std::string myImportError;

private:
    // Буфер для редактирования в ImGui (InputTextMultiline требует char*)
    static constexpr size_t kImportBufSize = 8192;
    char myImportBuf[kImportBufSize] = {};
    bool myImportBufInited = false;

    void SyncBufFromString();
    void SyncStringFromBuf();

    std::string myLlmResponse;

    static constexpr size_t kPromptBufSize = 2048;
    char myPromptBuf[kPromptBufSize] = {};
    bool myPromptBufInited = false;

    void SyncPromptBufFromString();
    void SyncPromptStringFromBuf();

    std::string myPrompt = "Построй прямоугольник 100 на 60, окружность радиусом 20 в центре и горизонтальную линию через центр";
    std::string myEndpoint = "http://localhost:1234";
    std::string myModel = ""; // позже подставим из /v1/models, пока можно руками

    std::string myLlmRequest;
    std::string myLlmRaw;
    std::string myLlmError;

    static constexpr size_t kSmallBuf = 1024;
    char myEndpointBuf[kSmallBuf]{};
    char myModelBuf[kSmallBuf]{};
    char myPromptBuf2[2048]{};
    bool myLlmBufInit = false;
};
