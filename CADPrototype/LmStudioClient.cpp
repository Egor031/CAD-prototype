#include "LmStudioClient.h"
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

static std::string Trim(std::string s)
{
    auto isws = [](unsigned char c) { return c == ' ' || c == '\n' || c == '\r' || c == '\t'; };
    while (!s.empty() && isws((unsigned char)s.front())) s.erase(s.begin());
    while (!s.empty() && isws((unsigned char)s.back())) s.pop_back();
    return s;
}

std::string LmStudioClient::DetectFirstModelId(const std::string& endpointBase, std::string& error)
{
    error.clear();
    httplib::Client cli(endpointBase);
    cli.set_connection_timeout(5);
    cli.set_read_timeout(10);

    auto res = cli.Get("/v1/models");
    if (!res)
    {
        error = "GET /v1/models failed: no response.";
        return {};
    }
    if (res->status < 200 || res->status >= 300)
    {
        error = "GET /v1/models HTTP " + std::to_string(res->status) + " body=" + res->body;
        return {};
    }

    json j;
    try { j = json::parse(res->body); }
    catch (...) { error = "Failed to parse /v1/models JSON."; return {}; }

    try
    {
        auto& data = j["data"];
        if (!data.is_array() || data.empty())
        {
            error = "No models in /v1/models.";
            return {};
        }
        return data[0]["id"].get<std::string>();
    }
    catch (...)
    {
        error = "Unexpected /v1/models format.";
        return {};
    }
}

LmStudioClient::Response LmStudioClient::ChatCompletions(const std::string& endpointBase,
    const std::string& modelId,
    const std::string& systemPrompt,
    const std::string& userPrompt,
    double temperature,
    int maxTokens)
{
    Response r;

    json req;
    req["model"] = modelId;
    req["temperature"] = temperature;
    req["max_tokens"] = maxTokens;
    req["messages"] = json::array({
        { {"role","system"}, {"content", systemPrompt} },
        { {"role","user"},   {"content", userPrompt} }
        });

    r.requestJson = req.dump(2);

    httplib::Client cli(endpointBase);
    cli.set_connection_timeout(5);
    cli.set_read_timeout(120);

    auto res = cli.Post("/v1/chat/completions", req.dump(), "application/json");
    if (!res)
    {
        r.error = "POST /v1/chat/completions failed: no response.";
        return r;
    }
    if (res->status < 200 || res->status >= 300)
    {
        r.error = "HTTP " + std::to_string(res->status) + " body=" + res->body;
        return r;
    }

    r.rawResponseJson = res->body;

    json ans;
    try { ans = json::parse(res->body); }
    catch (...) { r.error = "Failed to parse chat/completions JSON."; return r; }

    try
    {
        r.content = ans["choices"][0]["message"]["content"].get<std::string>();
        r.content = Trim(r.content);
        return r;
    }
    catch (...)
    {
        r.error = "Unexpected chat/completions format (no choices[0].message.content).";
        return r;
    }
}