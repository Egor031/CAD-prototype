#include "LlmAgent.h"

#include <cctype>
#include <sstream>

// очень простой парсер: ищем первое "id":123 в stateJson
unsigned long long LlmAgent::ExtractFirstIdFromState(const std::string& stateJson)
{
    const std::string key = "\"id\":";
    size_t pos = stateJson.find(key);
    if (pos == std::string::npos)
        return 0;

    pos += key.size();
    while (pos < stateJson.size() && std::isspace((unsigned char)stateJson[pos])) ++pos;

    unsigned long long id = 0;
    bool any = false;
    while (pos < stateJson.size() && std::isdigit((unsigned char)stateJson[pos]))
    {
        any = true;
        id = id * 10ULL + (unsigned long long)(stateJson[pos] - '0');
        ++pos;
    }
    return any ? id : 0;
}

std::string LlmAgent::EscapeJson(const std::string& s)
{
    std::ostringstream out;
    for (char c : s)
    {
        switch (c)
        {
        case '\\': out << "\\\\"; break;
        case '"':  out << "\\\""; break;
        case '\n': out << "\\n";  break;
        case '\r': out << "\\r";  break;
        case '\t': out << "\\t";  break;
        default:   out << c;      break;
        }
    }
    return out.str();
}

LlmAgent::Result LlmAgent::RunStub(const std::string& prompt, const std::string& stateJson)
{
    Result r;

    // request (просто для логов)
    {
        std::ostringstream ss;
        ss << "{"
            << "\"prompt\":\"" << EscapeJson(prompt) << "\","
            << "\"state\":" << stateJson
            << "}";
        r.requestJson = ss.str();
    }

    // заглушка поведения:
    // - если в prompt есть "удал" / "delete" => DeleteEntity первого id
    // - если есть "увелич" / "bigger" / "update" => UpdateBox первого id (dx*2)
    // - иначе => AddBox (как демонстрация)
    unsigned long long id = ExtractFirstIdFromState(stateJson);

    

    const std::string& p = prompt;

    auto contains = [&](const char* word)
        {
            return p.find(word) != std::string::npos;
        };

    if ((contains("удал") || contains("Удал") || contains("delete") || contains("Delete")) && id != 0)
    {
        r.responseJson = "[{\"type\":\"DeleteEntity\",\"id\":" + std::to_string(id) + "}]";
        return r;
    }

    if ((contains("увелич") || contains("Увелич") || contains("bigger") || contains("update")) && id != 0)
    {
        r.responseJson =
            "[{\"type\":\"UpdateBox\",\"id\":" + std::to_string(id) + ",\"dx\":100,\"dy\":50,\"dz\":50}]";
        return r;
    }

    // по умолчанию — добавить коробку
    r.responseJson = "[{\"type\":\"AddBox\",\"dx\":50,\"dy\":50,\"dz\":50}]";
    return r;
}