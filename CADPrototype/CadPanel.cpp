#include "CadPanel.h"

#include <imgui.h>

#include "History.h"
#include "Document.h"
#include "CmdAddBox.h"
#include "CommandFactory.h"

#include <memory>
#include <utility>
#include <cstring>

void CadPanel::SyncBufFromString()
{
    // кладём myImportJson в буфер один раз (или когда надо)
    std::memset(myImportBuf, 0, kImportBufSize);
#ifdef _MSC_VER
    strncpy_s(myImportBuf, kImportBufSize, myImportJson.c_str(), kImportBufSize - 1);
#else
    std::strncpy(myImportBuf, myImportJson.c_str(), kImportBufSize - 1);
#endif
    myImportBufInited = true;
}

void CadPanel::SyncStringFromBuf()
{
    myImportJson = std::string(myImportBuf);
}

bool CadPanel::Draw(History& history, Document& doc)
{
    bool changed = false;

    if (!myImportBufInited)
        SyncBufFromString();

    ImGui::Begin("CAD");

    // --- Basic ops
    ImGui::Button("Add Box");
    if (ImGui::IsItemActivated())
    {
        history.Apply(std::make_unique<CmdAddBox>(50, 50, 50), doc);
        myHistoryJson = history.ExportJson();
        changed = true;
    }

    ImGui::SameLine();

    ImGui::Button("Undo");
    if (ImGui::IsItemActivated())
    {
        history.Undo(doc);
        myHistoryJson = history.ExportJson();
        changed = true;
    }

    ImGui::SameLine();

    ImGui::Button("Redo");
    if (ImGui::IsItemActivated())
    {
        history.Redo(doc);
        myHistoryJson = history.ExportJson();
        changed = true;
    }

    static std::string stateJson;

    if (ImGui::Button("Refresh State JSON"))
    {
        stateJson = doc.ExportStateJson();
    }

    ImGui::BeginChild("state_box", ImVec2(520, 160), true);
    ImGui::TextUnformatted(stateJson.c_str());
    ImGui::EndChild();

    // --- History export
    if (ImGui::Button("Refresh History JSON"))
    {
        myHistoryJson = history.ExportJson();
    }

    ImGui::SameLine();

    if (ImGui::Button("Copy History"))
    {
        ImGui::SetClipboardText(myHistoryJson.c_str());
    }

    ImGui::BeginChild("history_box", ImVec2(520, 220), true);
    ImGui::TextUnformatted(myHistoryJson.c_str());
    ImGui::EndChild();

    // --- Import
    ImGui::Separator();
    ImGui::Text("Import JSON:");

    ImGui::InputTextMultiline("##import", myImportBuf, kImportBufSize, ImVec2(520, 160));
    SyncStringFromBuf();

    if (ImGui::Button("Apply JSON"))
    {
        myImportError.clear();

        std::string err;
        auto cmds = CommandFactory::ParseCommandArray(myImportJson, err);
        if (!err.empty())
        {
            myImportError = err;
        }
        else
        {
            for (auto& c : cmds)
            {
                history.Apply(std::move(c), doc);
            }

            myHistoryJson = history.ExportJson();
            changed = true;
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Copy Import"))
    {
        ImGui::SetClipboardText(myImportJson.c_str());
    }

    if (!myImportError.empty())
    {
        ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "%s", myImportError.c_str());
    }

    ImGui::End();

    return changed;
}
