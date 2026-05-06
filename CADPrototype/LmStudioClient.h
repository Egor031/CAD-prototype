#pragma once
#include <string>

class LmStudioClient
{
public:
    struct Response
    {
        std::string requestJson;
        std::string rawResponseJson;
        std::string content;  // то, что модель сказала (message.content)
        std::string error;
    };

    // GET /v1/models -> первый id (или error)
    static std::string DetectFirstModelId(const std::string& endpointBase, std::string& error);

    // POST /v1/chat/completions -> content
    static Response ChatCompletions(const std::string& endpointBase,
        const std::string& modelId,
        const std::string& systemPrompt,
        const std::string& userPrompt,
        double temperature = 0.1,
        int maxTokens = 1024);
};