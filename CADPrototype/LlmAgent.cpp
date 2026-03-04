#include "LlmAgent.h"

// External/
#include "httplib.h"
#include "json.hpp"

#include <sstream>

using json = nlohmann::json;

static std::string BuildSystemPrompt()
{
    // ЖЁСТКИЙ контракт: вернуть только JSON-массив команд
    return
        "You are a CAD assistant.\n"
        "Return ONLY a valid JSON array of commands.\n"
        "No explanations. No markdown. No code fences.\n"
        "Supported command types:\n"
        "- AddBox {type, dx, dy, dz}\n"
        "- AddBoxWithId {type, id, dx, dy, dz}\n"
        "- DeleteEntity {type, id}\n"
        "- UpdateBox {type, id, dx, dy, dz}\n";
}

LlmAgent::Result LlmAgent::RunLmStudioChat(const std::string& endpointBase,
    const std::string& model,
    const std::string& prompt,
    const std::string& stateJson)
{
    Result r;

    // --- 1) Собираем user message (prompt + state)
    std::ostringstream user;
    user << "User request:\n" << prompt << "\n\n"
        << "Current CAD state (JSON):\n" << stateJson << "\n\n"
        << "Return ONLY JSON array of commands.";

    // --- 2) Собираем запрос /v1/chat/completions
    json req;
    req["model"] = model.empty() ? "local-model" : model; // LM Studio обычно примет, но лучше реальный id
    req["temperature"] = 0.1;
    req["messages"] = json::array({
        { {"role","system"}, {"content", BuildSystemPrompt()} },
        { {"role","user"},   {"content", user.str()} }
        });

    r.requestJson = req.dump(2);

    // --- 3) HTTP POST
    httplib::Client cli(endpointBase);
    cli.set_connection_timeout(5);
    cli.set_read_timeout(120); // модель может думать долго локально

    auto res = cli.Post("/v1/chat/completions", req.dump(), "application/json");

    if (!res)
    {
        r.error = "LM Studio request failed: no response (server offline?)";
        return r;
    }
    if (res->status < 200 || res->status >= 300)
    {
        r.error = "LM Studio HTTP error: status=" + std::to_string(res->status) + " body=" + res->body;
        return r;
    }

    r.rawServerJson = res->body;

    // --- 4) Парсим ответ и вытаскиваем content
    json ans;
    try
    {
        ans = json::parse(res->body);
    }
    catch (...)
    {
        r.error = "Failed to parse LM Studio JSON response.";
        return r;
    }

    std::string content;
    try
    {
        // OpenAI chat format: choices[0].message.content
        content = ans["choices"][0]["message"]["content"].get<std::string>();
    }
    catch (...)
    {
        r.error = "LM Studio response has unexpected format (no choices[0].message.content).";
        return r;
    }

    // --- 5) Иногда модель всё равно оборачивает в ```...```. Уберём мягко.
    auto trim = [](std::string s)
        {
            auto isws = [](unsigned char c) { return c == ' ' || c == '\n' || c == '\r' || c == '\t'; };
            while (!s.empty() && isws((unsigned char)s.front())) s.erase(s.begin());
            while (!s.empty() && isws((unsigned char)s.back())) s.pop_back();
            return s;
        };

    content = trim(content);

    // убрать ```json ... ```
    if (content.rfind("```", 0) == 0)
    {
        auto p = content.find("\n");
        if (p != std::string::npos)
        {
            content = content.substr(p + 1);
            auto end = content.rfind("```");
            if (end != std::string::npos) content = content.substr(0, end);
            content = trim(content);
        }
    }

    r.responseJson = content; // <- это и есть commands_json
    return r;
}