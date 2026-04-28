//#pragma once
//#include <string>
//
//class LlmAgent
//{
//public:
//    struct Result
//    {
//        std::string requestJson;
//        std::string responseJson;   // commands_json (массив команд)
//        std::string rawServerJson;  // сырой ответ LM Studio (для дебага)
//        std::string error;
//    };
//
//    // Реальный вызов LM Studio (OpenAI-compatible)
//    static Result RunLmStudioChat(const std::string& endpointBase, // "http://localhost:1234"
//        const std::string& model,        // можно взять из /v1/models
//        const std::string& prompt,
//        const std::string& stateJson);
//
//    // Пока оставь stub, если хочешь
//    static Result RunStub(const std::string& prompt, const std::string& stateJson);
//};