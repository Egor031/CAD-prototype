#pragma once

#include <string>

class LlmAgent
{
public:
    struct Result
    {
        std::string requestJson;   // что "отправили" модели (для отладки)
        std::string responseJson;  // что "вернула" модель: JSON-массив команд
        std::string error;         // если не смогли сгенерить
    };

    // prompt — текст пользователя
    // stateJson — doc.ExportStateJson()
    static Result RunStub(const std::string& prompt, const std::string& stateJson);

private:
    static unsigned long long ExtractFirstIdFromState(const std::string& stateJson);
    static std::string EscapeJson(const std::string& s);
};