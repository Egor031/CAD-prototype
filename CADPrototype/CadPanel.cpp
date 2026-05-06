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
            "You are an assistant for creating 2D vector graphics and simple CAD geometry.\n"
            "Your task is to convert the user's natural language request into commands.\n"
            "\n"
            "Return ONLY a valid JSON array of commands.\n"
            "No explanations. No markdown. No code fences. No comments.\n"
            "\n"
            "Use the current CAD state to choose existing object ids when the user asks to modify or delete objects.\n"
            "If the user asks to create a new object, do not invent an id unless the command type explicitly requires id.\n"
            "\n"
            "Supported 2D vector command types:\n"
            "- AddLine {\"type\":\"AddLine\", \"x1\":number, \"y1\":number, \"x2\":number, \"y2\":number}\n"
            "- AddCircle {\"type\":\"AddCircle\", \"cx\":number, \"cy\":number, \"r\":number}\n"
            "- AddRectangle {\"type\":\"AddRectangle\", \"x\":number, \"y\":number, \"w\":number, \"h\":number}\n"
            "\n"
            "Supported 3D command types:\n"
            "- AddBox {\"type\":\"AddBox\", \"dx\":number, \"dy\":number, \"dz\":number}\n"
            "- AddBoxWithId {\"type\":\"AddBoxWithId\", \"id\":integer, \"dx\":number, \"dy\":number, \"dz\":number}\n"
            "- UpdateBox {\"type\":\"UpdateBox\", \"id\":integer, \"dx\":number, \"dy\":number, \"dz\":number}\n"
            "\n"
            "Supported common command types:\n"
            "- DeleteEntity {\"type\":\"DeleteEntity\", \"id\":integer}\n"
            "\n"
            "Rules:\n"
            "1. For 2D vector graphics, use AddLine, AddCircle, and AddRectangle.\n"
            "2. Coordinates are in the XY plane.\n"
            "3. A rectangle is defined by its lower-left point x,y and its width w and height h.\n"
            "4. A circle is defined by center cx,cy and radius r.\n"
            "5. If the user says 'inside the rectangle', compute approximate coordinates from the rectangle parameters.\n"
            "6. If the user says 'center', use the geometric center of the referenced object.\n"
            "7. If the user asks to delete an object, use DeleteEntity with the id from the current CAD state.\n"
            "8. If the request is ambiguous, make a reasonable simple assumption and still return JSON.\n"
            "\n"
            "Examples:\n"
            "User: Draw a rectangle 100 by 60.\n"
            "Answer: [{\"type\":\"AddRectangle\",\"x\":0,\"y\":0,\"w\":100,\"h\":60}]\n"
            "\n"
            "User: Draw a rectangle 100 by 60 and a circle with radius 20 in its center.\n"
            "Answer: [{\"type\":\"AddRectangle\",\"x\":0,\"y\":0,\"w\":100,\"h\":60},{\"type\":\"AddCircle\",\"cx\":50,\"cy\":30,\"r\":20}]\n"
            "\n"
            "User: Draw a horizontal line through the center of the rectangle 100 by 60.\n"
            "Answer: [{\"type\":\"AddRectangle\",\"x\":0,\"y\":0,\"w\":100,\"h\":60},{\"type\":\"AddLine\",\"x1\":0,\"y1\":30,\"x2\":100,\"y2\":30}]\n";

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
