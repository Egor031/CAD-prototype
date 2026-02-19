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

    std::string myImportJson =
        "[\n"
        "  {\"type\":\"AddBox\",\"dx\":50,\"dy\":50,\"dz\":50}\n"
        "]";

    std::string myImportError;

private:
    // Буфер для редактирования в ImGui (InputTextMultiline требует char*)
    static constexpr size_t kImportBufSize = 8192;
    char myImportBuf[kImportBufSize] = {};
    bool myImportBufInited = false;

    void SyncBufFromString();
    void SyncStringFromBuf();
};
