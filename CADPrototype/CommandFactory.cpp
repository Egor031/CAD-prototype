#include "CommandFactory.h"

#include "ICommand.h"
#include "CmdAddBox.h"

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
                // конец массива
                return true;
            }

            ++i;
        }

        error = "Expected ']' at end of JSON array.";
        return false;
    }

    static bool TryParseAddBox(const std::string& obj, std::unique_ptr<ICommand>& outCmd, std::string& error)
    {
        // Минимально: ищем type и dx/dy/dz числа (int/float), порядок ключей может быть любым
        std::smatch m;

        std::regex typeRe("\"type\"\\s*:\\s*\"([^\"]+)\"");

            if (!std::regex_search(obj, m, typeRe))
            {
                error = "Command object has no \"type\" field.";
                return false;
            }

        const std::string type = m[1].str();
        if (type != "AddBox")
        {
            error = "Unsupported command type: " + type;
            return false;
        }

        auto findNumber = [&](const char* key, double& outVal) -> bool
            {
                std::regex re(std::string("\"") + key + "\"\\s*:\\s*(-?\\d+(\\.\\d+)?)");

                std::smatch mm;
                if (!std::regex_search(obj, mm, re))
                    return false;
                outVal = std::stod(mm[1].str());
                return true;
            };

        double dx = 0, dy = 0, dz = 0;
        if (!findNumber("dx", dx) || !findNumber("dy", dy) || !findNumber("dz", dz))
        {
            error = "AddBox requires numeric fields dx, dy, dz.";
            return false;
        }

        outCmd = std::make_unique<CmdAddBox>(dx, dy, dz);
        return true;
    }

    static bool ParseOneCommand(const std::string& obj, std::unique_ptr<ICommand>& outCmd, std::string& error)
    {
        // Сейчас поддерживаем только AddBox
        return TryParseAddBox(obj, outCmd, error);
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
