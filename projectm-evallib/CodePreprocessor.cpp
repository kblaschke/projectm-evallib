#include "CodePreprocessor.hpp"

#include <cstdlib>
#include <string>
#include <cstddef>

std::string CodePreprocessor::PreprocessCode(const std::string& code)
{
    std::string preprocessedCode{ code };

    // First strip all whitespace. Don't need to care about "- -" turning into "--", as the syntax doesn't support
    // pre-/post-increment ops.
    StripWhitespace(preprocessedCode);

    // Replace undocumented special constants: $Xn, $PI, $E and $PHI
    ReplaceConstant(preprocessedCode, "$PI", "3.141592653589793");
    ReplaceConstant(preprocessedCode, "$E", "2.71828183");
    ReplaceConstant(preprocessedCode, "$PHI", "1.61803399");
    ReplaceHexValues(preprocessedCode);

    // Replace operators implemented as internal functions, e.g. assignment ops.

    // Replace other special operators
    ReplaceTernaryOperators(preprocessedCode);

    return preprocessedCode;
}

void CodePreprocessor::StripWhitespace(std::string& code)
{
    size_t pos{ 0 };
    while ((pos = code.find_first_of(" \t\v\r\n", pos)) != std::string::npos)
    {
        code.erase(pos, 1);
    }
}

void CodePreprocessor::ReplaceConstant(std::string& code, const std::string& constant, const std::string& value)
{
    size_t pos{ 0 };
    while ((pos = code.find(constant)) != std::string::npos)
    {
        code.replace(pos, constant.length(), value);
    }
}

void CodePreprocessor::ReplaceHexValues(std::string& code)
{
    size_t pos{ 0 };
    while ((pos = code.find("$X")) != std::string::npos)
    {
        const char* startpos{ code.data() + pos };
        char* endpos{ nullptr };
        auto decimalValue = strtoull(startpos + 2, &endpos, 16);
        code.replace(pos, endpos - startpos, std::to_string(decimalValue));
    }
}

void CodePreprocessor::ReplaceTernaryOperators(std::string& code)
{
    size_t pos{ 0 };
    while ((pos = code.find('?')) != std::string::npos)
    {
        size_t expressionLength{ 1 };

        // Find left side
        auto leftParam = GetLeftParam(code, pos - 1, "=:(,;?%");

        if (leftParam.empty())
        {
            // Todo: throw if broken?
            break;
        }

        expressionLength += leftParam.length();

        // Find right side expression until :
        auto rightParam1 = GetRightParam(code, pos + 1, ",);:?");
        size_t rightParam1Length = rightParam1.size();
        expressionLength += rightParam1.length();

        if (!rightParam1.empty())
        {
            ReplaceTernaryOperators(rightParam1);
        }
        else
        {
            rightParam1 = "0";
        }

        std::string rightParam2;
        auto rightParam2Start = pos + rightParam1Length + 1;
        if (rightParam2Start < code.length() && code[rightParam2Start] == ':')
        {
            rightParam2 = GetRightParam(code, rightParam2Start + 1, ",);");
            expressionLength += rightParam2.length() + 1;
            if (!rightParam2.empty())
            {
                size_t originalLength = rightParam2.size();
                ReplaceTernaryOperators(rightParam2);
            }
        }

        if (rightParam2.empty())
        {
            rightParam2 = "0";
        }

        // Replace everything with "if(leftParam,rightParam1,rightParam2)"
        std::string replacement;
        replacement.reserve(leftParam.length() + rightParam1.length() + rightParam2.length() + 7);
        replacement.append("if(").append(leftParam).append(",")
            .append(rightParam1).append(",")
            .append(rightParam2).append(")");

        code.replace(pos - leftParam.length(), expressionLength, replacement);
    }
}

std::string CodePreprocessor::GetRightParam(std::string& code, size_t startOffset, const std::string& terminatorChars)
{
    size_t pos{startOffset};
    int parenthesisLevel{0};
    int bracketsLevel{0};
    int ternaryLevel{0};

    while (pos < code.length())
    {
        auto ch = code[pos];

        switch (ch)
        {
            case '(':
                ++parenthesisLevel;
                ++pos;
                continue;

            case ')':
                if (parenthesisLevel > 0)
                {
                    --parenthesisLevel;
                    ++pos;
                    continue;
                }
                break;

            case '[':
                ++bracketsLevel;
                ++pos;
                continue;

            case ']':
                if (bracketsLevel > 0)
                {
                    --bracketsLevel;
                    ++pos;
                    continue;
                }
                break;

            case '?':
                // A ternary counts as a single expression.
                ++ternaryLevel;
                ++pos;
                continue;

            case ':':
                if (ternaryLevel > 0)
                {
                    --ternaryLevel;
                    ++pos;
                    continue;
                }
                break;
        }

        if (parenthesisLevel == 0 && bracketsLevel == 0 && ternaryLevel == 0)
        {
            if (terminatorChars.find(ch) != std::string::npos)
            {
                return code.substr(startOffset, pos - startOffset);
            }
        }

        ++pos;
    }

    return code.substr(startOffset, code.length() - startOffset);
    ;
}

std::string CodePreprocessor::GetLeftParam(std::string& code, size_t startOffset, const std::string& terminatorChars)
{
    size_t pos{startOffset};
    int parenthesisLevel{0};
    int bracketsLevel{0};
    int ternaryLevel{0};

    while (pos > 0)
    {
        auto ch = code[pos];

        switch (ch)
        {
            case ')':
                ++parenthesisLevel;
                --pos;
                continue;

            case '(':
                if (parenthesisLevel > 0)
                {
                    --parenthesisLevel;
                    --pos;
                    continue;
                }
                break;

            case ']':
                ++bracketsLevel;
                --pos;
                continue;

            case '[':
                if (bracketsLevel > 0)
                {
                    --bracketsLevel;
                    --pos;
                    continue;
                }
                break;

            case ':':
                // A ternary counts as a single expression.
                ++ternaryLevel;
                --pos;
                continue;

            case '?':
                if (ternaryLevel > 0)
                {
                    --ternaryLevel;
                    --pos;
                    continue;
                }
                break;
        }

        if (parenthesisLevel == 0 && bracketsLevel == 0 && ternaryLevel == 0)
        {
            if (terminatorChars.find(ch) != std::string::npos)
            {
                auto ret = code.substr(pos + 1, startOffset - pos);
                return ret;
            }
        }

        --pos;
    }

    return code.substr(0, startOffset + 1);
}
