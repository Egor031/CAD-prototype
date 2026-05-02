#include "CadPanel.h"

#include <imgui.h>

#include "History.h"
#include "Document.h"
#include "CmdAddBox.h"
#include "CommandFactory.h"
#include "CmdDeleteEntity.h"

#include <memory>
#include <utility>
#include <cstring>

#include "LmStudioClient.h"

void CadPanel::SyncPromptBufFromString()
{
    std::memset(myPromptBuf, 0, kPromptBufSize);
#ifdef _MSC_VER
    strncpy_s(myPromptBuf, kPromptBufSize, myPrompt.c_str(), kPromptBufSize - 1);
#else
    std::strncpy(myPromptBuf, myPrompt.c_str(), kPromptBufSize - 1);
#endif
    myPromptBufInited = true;
}

void CadPanel::SyncPromptStringFromBuf()
{
    myPrompt = std::string(myPromptBuf);
}

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

    if (!myPromptBufInited)
        SyncPromptBufFromString();

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

    if (ImGui::Button("Delete selected"))
    {
        EntityId selectedId = 0;

        if (doc.TryGetSelectedEntityId(selectedId))
        {
            history.Apply(std::make_unique<CmdDeleteEntity>(selectedId), doc);
            changed = true;
        }
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

    ImGui::Separator();
    ImGui::Text("LLM (stub):");

    ImGui::InputTextMultiline("##prompt", myPromptBuf, kPromptBufSize, ImVec2(520, 80));
    SyncPromptStringFromBuf();

    if (ImGui::Button("Run LLM"))
    {
        myLlmError.clear();

        const std::string state = doc.ExportStateJson();

        std::string systemPrompt =
            "You are a CAD assistant.\n"
            "Return ONLY a valid JSON array of commands.\n"
            "No explanations. No markdown. No code fences.\n"
            "Supported command types:\n"
            "- AddBox {type, dx, dy, dz}\n"
            "- AddBoxWithId {type, id, dx, dy, dz}\n"
            "- DeleteEntity {type, id}\n"
            "- UpdateBox {type, id, dx, dy, dz}\n";

        std::string userPrompt =
            "User request:\n" + myPrompt + "\n\n"
            "Current CAD state (JSON):\n" + state + "\n\n"
            "Return ONLY JSON array of commands.";

        auto res = LmStudioClient::ChatCompletions(
            myEndpoint,
            myModel,
            systemPrompt,
            userPrompt,
            0.1,
            512
        );

        myLlmRequest = res.requestJson;
        myLlmRaw = res.rawResponseJson;
        myLlmResponse = res.content;

        // применяем команды
        std::string err;
        auto cmds = CommandFactory::ParseCommandArray(myLlmResponse, err);
        if (!err.empty())
        {
            myLlmError = err;
        }
        else
        {
            for (auto& c : cmds)
                history.Apply(std::move(c), doc);

            myHistoryJson = history.ExportJson();
            changed = true;
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Copy LLM Response"))
    {
        ImGui::SetClipboardText(myLlmResponse.c_str());
    }

    ImGui::Text("LLM Request:");
    ImGui::BeginChild("llm_req", ImVec2(520, 80), true);
    ImGui::TextUnformatted(myLlmRequest.c_str());
    ImGui::EndChild();

    ImGui::Text("LLM Response (commands_json):");
    ImGui::BeginChild("llm_resp", ImVec2(520, 80), true);
    ImGui::TextUnformatted(myLlmResponse.c_str());
    ImGui::EndChild();

    if (!myLlmError.empty())
    {
        ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "LLM Apply Error: %s", myLlmError.c_str());
    }

    ImGui::End();

    return changed;
}
