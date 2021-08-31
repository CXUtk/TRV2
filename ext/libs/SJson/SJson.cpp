#include "SJson.h"
#include <cassert>

namespace SJson {
    static int lineNumber = 1;
    static std::string::const_iterator lastLineStart;
    static std::string::const_iterator stringEnd;
    static std::shared_ptr<SJsonNode> parse_number(std::string::const_iterator& it);
    static std::shared_ptr<SJsonNode> parse_string(std::string::const_iterator& it);
    static std::shared_ptr<SJsonNode> parse_null(std::string::const_iterator& it);
    static std::shared_ptr<SJsonNode> parse_true(std::string::const_iterator& it);
    static std::shared_ptr<SJsonNode> parse_false(std::string::const_iterator& it);
    static std::shared_ptr<SJsonNode> parse_array(std::string::const_iterator& it);
    static std::shared_ptr<SJsonNode> parse_object(std::string::const_iterator& it);
    static std::shared_ptr<SJsonNode> parse_value(std::string::const_iterator& it);

    std::string getText(std::string::const_iterator it) {
        static char s[10];
        if (it == stringEnd) return "EOF";
        s[0] = *it;
        s[1] = '\0';
        return s;
    }

    static bool isEnd(std::string::const_iterator& it) {
        return stringEnd == it;
    }


    static void newLine(std::string::const_iterator it) {
        lineNumber++;
        lastLineStart = it;
    }

    static void expect(std::string::const_iterator& it, char ch) {
        if (it == stringEnd || *it != ch) {
            throw ExpectTokenError(lineNumber, it - lastLineStart, std::string(1, ch), getText(it));
        }
        if (*it == '\n') newLine(it);
        it++;
    }


    static void strip_white_space(std::string::const_iterator& it) {
        while (it != stringEnd && (*it == ' ' || *it == '\n' || *it == '\r' || *it == '\t')) {
            if (*it == '\n') newLine(it);
            it++;
        }
    }

    std::string SJson::SJsonGetNodeTypeName(SJsonNodeType type) {
        switch (type) {
        case SJsonNodeType::JSON_NULL:
            return "Null";
        case SJsonNodeType::JSON_BOOL:
            return "Bool";
        case SJsonNodeType::JSON_INT:
            return "Integer";
        case SJsonNodeType::JSON_FLOAT:
            return "Float";
        case SJsonNodeType::JSON_STRING:
            return "String";
        case SJsonNodeType::JSON_ARRAY:
            return "Array";
        case SJsonNodeType::JSON_OBJECT:
            return "Object";
        default:
            return "Unknown";
        }
    }
    static std::shared_ptr<SJsonNode> parse_number(std::string::const_iterator& it) {
        // [-] [INT] (.) (FRAC) (EXP)
        int stage = 0;
        bool neg = false;

        if (it != stringEnd && *it == '-') {
            neg = true;
            it++;
        }

        static char intStr[30];
        int intIndex = 0;

        static char fracStr[30];
        int fracIndex = 0;

        static char expStr[30];
        int expIndex = 0;
        bool expNeg = false;
        bool signChecked = false;


        while (it != stringEnd) {
            if (stage == 0) {
                if (isdigit(*it)) {
                    intStr[intIndex++] = *it;
                }
                else if (*it == '.') {
                    stage = 1;
                }
                else if (tolower(*it) == 'e') {
                    stage = 2;
                }
                else {
                    break;
                }
                it++;
            }
            else if (stage == 1) {
                if (isdigit(*it)) {
                    fracStr[fracIndex++] = *it;
                }
                else if (*it == '.') {
                    throw InvalidValueError(lineNumber, it - lastLineStart);
                }
                else if (tolower(*it) == 'e') {
                    stage = 2;
                }
                else {
                    break;
                }
                it++;
            }
            else {
                if (!signChecked) {
                    if (it != stringEnd) {
                        if (*it == '-') {
                            expNeg = true;
                            it++;
                        }
                        else if (*it == '+') {
                            expNeg = false;
                            it++;
                        }
                    }
                    signChecked = true;
                }
                if (isdigit(*it)) {
                    expStr[expIndex++] = *it;
                }
                else if (*it == '.') {
                    throw InvalidValueError(lineNumber, it - lastLineStart);
                }
                else if (tolower(*it) == 'e') {
                    throw InvalidValueError(lineNumber, it - lastLineStart);
                }
                else {
                    break;
                }
                it++;
            }
        }
        if (!intIndex && !fracIndex && !expIndex) {
            throw InvalidValueError(lineNumber, it - lastLineStart);
        }
        if (intStr[0] == '0' && intIndex > 1) {
            throw InvalidValueError(lineNumber, it - lastLineStart);
        }
        if (intIndex && !fracIndex && !expIndex) {
            ll value = 0;
            for (int i = 0; i < intIndex; i++) {
                value *= 10;
                value += intStr[i] - '0';
            }
            return std::make_shared<SJsonIntNode>(neg ? -value : value);
        }
        else {
            double value = 0;
            for (int i = 0; i < intIndex; i++) {
                value *= 10.0;
                value += intStr[i] - '0';
            }

            if (fracIndex) {
                double t = 10;
                for (int i = 0; i < fracIndex; i++) {
                    value += (fracStr[i] - '0') / t;
                    t *= 10;
                }
            }
            if (expIndex) {
                ll expV = 0;
                for (int i = 0; i < expIndex; i++) {
                    expV *= 10;
                    expV += expStr[i] - '0';
                }
                value *= std::pow(10.0, expNeg ? -expV : expV);
            }
            return std::make_shared<SJsonFloatNode>(neg ? -value : value);
        }
    }

    static std::shared_ptr<SJsonNode> parse_array(std::string::const_iterator& it) {
        std::vector<std::shared_ptr<SJsonNode>> elements;
        expect(it, '[');
        strip_white_space(it);
        if (it != stringEnd && *it == ']') {
            it++;
            return std::make_shared<SJsonArrayNode>(elements);
        }
        bool end = false;
        while (it != stringEnd) {
            strip_white_space(it);
            if (*it == ']') break;
            auto e = parse_value(it);
            elements.push_back(e);
            strip_white_space(it);
            if (*it == ',') {
                it++;
            }
            else if (*it == ']') {
                it++;
                end = true;
                break;
            }
            else {
                throw RootNotSingularError(lineNumber, it - lastLineStart);
            }
        }
        if (!end) {
            throw ExpectValueError(lineNumber, it - lastLineStart);
        }
        return std::make_shared<SJsonArrayNode>(elements);
    }

    static std::shared_ptr<SJsonNode> parse_object(std::string::const_iterator& it) {
        std::map<std::string, std::shared_ptr<SJsonNode>> objMap;
        expect(it, '{');
        strip_white_space(it);
        if (it != stringEnd && *it == '}') {
            it++;
            return std::make_shared<SJsonObjectNode>(objMap);
        }
        bool end = false;
        while (it != stringEnd) {
            strip_white_space(it);
            if (*it == '}')break;
            auto name = parse_string(it);
            strip_white_space(it);
            expect(it, ':');
            strip_white_space(it);
            auto e = parse_value(it);
            objMap.insert(std::pair<std::string, std::shared_ptr<SJsonNode>>(name->GetString(), e));
            strip_white_space(it);
            if (*it == ',') {
                it++;
            }
            else if (*it == '}') {
                it++;
                end = true;
                break;
            }
            else {
                throw RootNotSingularError(lineNumber, it - lastLineStart);
            }
        }
        if (!end) {
            throw ExpectValueError(lineNumber, it - lastLineStart);
        }
        return std::make_shared<SJsonObjectNode>(objMap);
    }

    static std::shared_ptr<SJsonNode> parse_string(std::string::const_iterator& it) {
        std::string str;
        expect(it, '"');
        while (it != stringEnd && *it != '"') {
            if (*it == '\\') {
                it++;
                if (it == stringEnd) break;
                if (*it == '"') {
                    str.push_back('\"');
                }
                else if (*it == '\\') {
                    str.push_back('\\');
                }
                else if (*it == 'b') {
                    str.push_back('\b');
                }
                else if (*it == 'f') {
                    str.push_back('\f');
                }
                else if (*it == 'n') {
                    str.push_back('\n');
                }
                else if (*it == 'r') {
                    str.push_back('\r');
                }
                else if (*it == 't') {
                    str.push_back('\t');
                }
                else {
                    throw UnexpectedTokenError(lineNumber, it - lastLineStart, getText(it));
                }
            }
            else {
                str.push_back(*it);
            }
            it++;
        }
        expect(it, '"');
        return std::make_shared<SJsonStringNode>(str);
    }


    static std::shared_ptr<SJsonNode> parse_true(std::string::const_iterator& it) {
        static const char trueStr[5] = "true";
        for (int i = 0; i < 4; i++) {
            if (it == stringEnd || *it != trueStr[i]) {
                throw UnexpectedTokenError(lineNumber, it - lastLineStart, getText(it));
            }
            it++;
        }
        return std::make_shared<SJsonBoolNode>(true);
    }

    static std::shared_ptr<SJsonNode> parse_false(std::string::const_iterator& it) {
        static const char trueStr[6] = "false";
        for (int i = 0; i < 5; i++) {
            if (it == stringEnd || *it != trueStr[i]) {
                throw UnexpectedTokenError(lineNumber, it - lastLineStart, getText(it));
            }
            it++;
        }
        return std::make_shared<SJsonBoolNode>(false);
    }


    std::shared_ptr<SJsonNode> parse_null(std::string::const_iterator& it) {
        static const char nullStr[5] = "null";
        for (int i = 0; i < 4; i++) {
            if (it == stringEnd || *it != nullStr[i]) {
                throw UnexpectedTokenError(lineNumber, it - lastLineStart, getText(it));
            }
            it++;
        }
        return std::make_shared<SJsonNullNode>();
    }

    std::shared_ptr<SJsonNode> parse_value(std::string::const_iterator& it) {
        if (it == stringEnd) {
            throw ExpectValueError(lineNumber, it - lastLineStart);
        }
        auto ch = *it;
        if (ch == 'n') {
            return parse_null(it);
        }
        else if (ch == 't') {
            return parse_true(it);
        }
        else if (ch == 'f') {
            return parse_false(it);
        }
        else if (ch == '"') {
            return parse_string(it);
        }
        else if (ch == '[') {
            return parse_array(it);
        }
        else if (ch == '{') {
            return parse_object(it);
        }
        else {
            return parse_number(it);
        }
    }

    std::shared_ptr<SJsonNode> SJson::SJsonParse(const std::string& text) {
        lineNumber = 1;
        auto ptr = text.begin();
        lastLineStart = ptr;
        stringEnd = text.end();
        strip_white_space(ptr);
        auto result = parse_value(ptr);
        strip_white_space(ptr);
        if (!isEnd(ptr))
            throw RootNotSingularError(lineNumber, ptr - lastLineStart);
        return result;
    }
}
