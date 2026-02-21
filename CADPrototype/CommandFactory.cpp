#include "CommandFactory.h"

#include "ICommand.h"
#include "CmdAddBox.h"
#include "CmdDeleteEntity.h"
#include "Document.h"
#include "CmdAddBoxWithId.h"
#include "CmdUpdateBox.h"

#include <cctype>
#include <regex>
#include <string>
#include <vector>


namespace
{
    static void SkipWs(const std::string& s, size_t& i)
    {
        while (i < s.size() && std::isspace((unsigned char)s[i])) ++i;
    }

    // Извлекает из [ ... ] список строк-объектов { ... } верхнего уровня.
    // Умеет игнорировать фигурные скобки внутри строк.
    static bool ExtractTopLevelObjects(const std::string& json,
        std::vector<std::string>& outObjects,
        std::string& error)
    {
        size_t i = 0;
        SkipWs(json, i);
        if (i >= json.size() || json[i] != '[')
        {
            error = "Expected '[' at start of JSON array.";
            return false;
        }
        ++i;

        bool inString = false;
        bool escape = false;
        int braceDepth = 0;

        size_t objStart = std::string::npos;

        while (i < json.size())
        {
            char c = json[i];

            if (inString)
            {
                if (escape)
                {
                    escape = false;
                }
                else if (c == '\\')
                {
                    escape = true;
                }
                else if (c == '"')
                {
                    inString = false;
                }
                ++i;
                continue;
            }
            else
            {
                if (c == '"')
                {
                    inString = true;
                    ++i;
                    continue;
                }
            }

            if (c == '{')
            {
                if (braceDepth == 0)
                    objStart = i;
                ++braceDepth;
            }
            else if (c == '}')
            {
                --braceDepth;
                if (braceDepth < 0)
                {
                    error = "Unexpected '}' in JSON.";
                    return false;
                }
                if (braceDepth == 0 && objStart != std::string::npos)
                {
                    outObjects.push_back(json.substr(objStart, i - objStart + 1));
                    objStart = std::string::npos;
                }
            }
            else if (c == ']')
            {
                if (braceDepth != 0)
                {
                    error = "Unclosed '{' before ']'.";
                    return false;
                }
                return true;
            }

            ++i;
        }

        error = "Expected ']' at end of JSON array.";
        return false;
    }

    static bool ExtractType(const std::string& obj, std::string& outType)
    {
        std::smatch m;
        std::regex typeRe("\"type\"\\s*:\\s*\"([^\"]+)\"");
        if (!std::regex_search(obj, m, typeRe))
            return false;
        outType = m[1].str();
        return true;
    }

    static bool TryGetNumber(const std::string& obj, const char* key, double& outVal)
    {
        std::regex re(std::string("\"") + key + "\"\\s*:\\s*(-?\\d+(\\.\\d+)?)");
        std::smatch mm;
        if (!std::regex_search(obj, mm, re))
            return false;
        outVal = std::stod(mm[1].str());
        return true;
    }

    static bool TryGetU64(const std::string& obj, const char* key, uint64_t& outVal)
    {
        // только неотрицательные (id)
        std::regex re(std::string("\"") + key + "\"\\s*:\\s*(\\d+)");
        std::smatch mm;
        if (!std::regex_search(obj, mm, re))
            return false;
        outVal = static_cast<uint64_t>(std::stoull(mm[1].str()));
        return true;
    }

    static bool TryParseAddBox(const std::string& obj, std::unique_ptr<ICommand>& outCmd, std::string& error)
    {
        double dx = 0, dy = 0, dz = 0;
        if (!TryGetNumber(obj, "dx", dx) || !TryGetNumber(obj, "dy", dy) || !TryGetNumber(obj, "dz", dz))
        {
            error = "AddBox requires numeric fields dx, dy, dz.";
            return false;
        }

        outCmd = std::make_unique<CmdAddBox>(dx, dy, dz);
        return true;
    }

    static bool TryParseUpdateBox(const std::string& obj, std::unique_ptr<ICommand>& outCmd, std::string& error)
    {
        uint64_t id = 0;
        double dx = 0, dy = 0, dz = 0;

        if (!TryGetU64(obj, "id", id) || !TryGetNumber(obj, "dx", dx) || !TryGetNumber(obj, "dy", dy) || !TryGetNumber(obj, "dz", dz))
        {
            error = "UpdateBox requires id, dx, dy, dz.";
            return false;
        }

        outCmd = std::make_unique<CmdUpdateBox>((EntityId)id, dx, dy, dz);
        return true;
    }


    static bool TryParseDeleteEntity(const std::string& obj, std::unique_ptr<ICommand>& outCmd, std::string& error)
    {
        uint64_t id = 0;
        if (!TryGetU64(obj, "id", id))
        {
            error = "DeleteEntity requires numeric field id.";
            return false;
        }

        outCmd = std::make_unique<CmdDeleteEntity>((EntityId)id);
        return true;
    }

    static bool ParseOneCommand(const std::string& obj, std::unique_ptr<ICommand>& outCmd, std::string& error)
    {
        std::string type;
        if (!ExtractType(obj, type))
        {
            error = "Command object has no \"type\" field.";
            return false;
        }

        if (type == "AddBox")
            return TryParseAddBox(obj, outCmd, error);

        if (type == "DeleteEntity")
            return TryParseDeleteEntity(obj, outCmd, error);

        if (type == "AddBoxWithId")
        {
            uint64_t id = 0; double dx = 0, dy = 0, dz = 0;
            if (!TryGetU64(obj, "id", id) || !TryGetNumber(obj, "dx", dx) || !TryGetNumber(obj, "dy", dy) || !TryGetNumber(obj, "dz", dz))
            {
                error = "AddBoxWithId requires id, dx, dy, dz."; return false;
            }

            outCmd = std::make_unique<CmdAddBoxWithId>((EntityId)id, dx, dy, dz);
            return true;
        }

        if (type == "UpdateBox")
            return TryParseUpdateBox(obj, outCmd, error);

        error = "Unsupported command type: " + type;
        return false;
    }



}
std::vector<std::unique_ptr<ICommand>> CommandFactory::ParseCommandArray(
    const std::string& json,
    std::string& error)
{
    std::vector<std::unique_ptr<ICommand>> result;
    error.clear();

    std::vector<std::string> objects;
    if (!ExtractTopLevelObjects(json, objects, error))
        return {};

    for (size_t i = 0; i < objects.size(); ++i)
    {
        std::unique_ptr<ICommand> cmd;
        std::string cmdErr;
        if (!ParseOneCommand(objects[i], cmd, cmdErr))
        {
            error = "Command #" + std::to_string(i) + " parse error: " + cmdErr;
            return {};
        }
        result.push_back(std::move(cmd));
    }

    return result;
}


