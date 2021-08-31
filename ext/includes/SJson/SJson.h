#pragma once

#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <vector>
#include <memory>

using ll = long long int;

namespace SJson {

    // 各类异常，枚举类
    enum class SJsonNodeType { JSON_NULL, JSON_BOOL, JSON_INT, JSON_FLOAT, JSON_STRING, JSON_ARRAY, JSON_OBJECT };

    std::string SJsonGetNodeTypeName(SJsonNodeType type);

    class ParseError : public std::exception {
    public:
        explicit ParseError(const char* name, int line, int col) : std::exception(name), _line(line), _col(col) {
        }
        int GetLine() const { return _line; }
        int GetCol() const { return _col; }
    private:
        int _line, _col;
    };

    class RootNotSingularError : public ParseError {
    public:
        explicit RootNotSingularError(int line, int col) : ParseError("RootNotSingularError", line, col) {}
    };

    class ExpectValueError : public ParseError {
    public:
        explicit ExpectValueError(int line, int col) : ParseError("ExpectValueError", line, col) {}
    };

    class InvalidValueError : public ParseError {
    public:
        explicit InvalidValueError(int line, int col) : ParseError("InvalidValueError", line, col) {}
    };

    class UnexpectedTokenError : public ParseError {
    public:
        explicit UnexpectedTokenError(int line, int col, const std::string& c) : ParseError("UnexpectedTokenError", line, col), token(c) {}
        std::string token;
    };

    class ExpectTokenError : public ParseError {
    public:
        explicit ExpectTokenError(int line, int col, const std::string& c, const std::string& cur) : ParseError("ExpectTokenError", line, col), token(c), curToken(cur) {}
        std::string token, curToken;
    };

    class OperationError : public std::exception {
    public:
        explicit OperationError(const char* name) : std::exception(name) {}
    };

    class ConversionError : public OperationError {
    public:
        explicit ConversionError(const std::string& fr, const std::string& to) : OperationError("Conversion Error"),
            from(fr), to(to) {
        }
        std::string from, to;
    };

    class MissingMemberError : public OperationError {
    public:
        explicit MissingMemberError(const std::string& s) : OperationError("Member Not Found Error"),
            member(s) {
        }
        std::string member;
    };


    /// <summary>
    /// SJsonNode 是Json节点的基类，包含各种取值判断操作
    /// </summary>
    class SJsonNode {
    public:
        SJsonNode() = default;
        virtual ~SJsonNode() = 0 {}

        virtual SJsonNodeType GetType() const = 0;

        virtual bool IsNull() const { return false; }

        virtual ll GetInt() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_INT));
        }
        virtual double GetFloat() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_FLOAT));
        }
        virtual bool GetBool() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_BOOL));
        }
        virtual std::string GetString() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_STRING));
        }

        // Array 操作
        virtual std::vector<std::shared_ptr<SJsonNode>>::const_iterator begin() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_ARRAY));
        }
        virtual std::vector<std::shared_ptr<SJsonNode>>::const_iterator end() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_ARRAY));
        }
        virtual std::size_t arraySize() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_ARRAY));
        }
        virtual std::shared_ptr<SJsonNode> ElementAt(int index) const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_ARRAY));
        }

        // Object 操作
        virtual const std::shared_ptr<SJsonNode>& GetMember(const std::string& name) const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_OBJECT));
        }

        virtual bool HasMember(const std::string& name) const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_OBJECT));
        }

        virtual std::map<std::string, std::shared_ptr<SJsonNode>>::const_iterator beginProperties() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_OBJECT));
        }
        virtual std::map<std::string, std::shared_ptr<SJsonNode>>::const_iterator endProperties() const {
            throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_OBJECT));
        }
    };


    class SJsonObjectNode : public SJsonNode {
    public:
        SJsonObjectNode(const std::map<std::string, std::shared_ptr<SJsonNode>>& items) : _value(items) {}
        ~SJsonObjectNode() override {}

        SJsonNodeType GetType() const override { return SJsonNodeType::JSON_OBJECT; };
        const std::shared_ptr<SJsonNode>& GetMember(const std::string& name) const override {
            auto v = _value.find(name);
            if (v == _value.end()) throw MissingMemberError(name);
            return v->second;
        }
        bool HasMember(const std::string& name) const override {
            return _value.find(name) != _value.end();
        }
        std::map<std::string, std::shared_ptr<SJsonNode>>::const_iterator beginProperties() const {
            return _value.cbegin();
        }
        std::map<std::string, std::shared_ptr<SJsonNode>>::const_iterator endProperties() const {
            return _value.cend();
        }
    private:
        std::map<std::string, std::shared_ptr<SJsonNode>> _value;
    };


    class SJsonArrayNode : public SJsonNode {
    public:
        SJsonArrayNode(const std::vector<std::shared_ptr<SJsonNode>>& items) : _value(items) {}
        ~SJsonArrayNode() override {}

        SJsonNodeType GetType() const override { return SJsonNodeType::JSON_ARRAY; };

        std::size_t arraySize() const override { return _value.size(); }
        std::vector<std::shared_ptr<SJsonNode>>::const_iterator begin() const override {
            return _value.cbegin();
        }
        std::vector<std::shared_ptr<SJsonNode>>::const_iterator end() const override {
            return _value.cend();
        }
        std::shared_ptr<SJsonNode> ElementAt(int index) const override {
            return _value[index];
        }
    private:
        std::vector<std::shared_ptr<SJsonNode>> _value;
    };

    class SJsonNullNode : public SJsonNode {
    public:
        SJsonNullNode() = default;
        ~SJsonNullNode() override {}

        SJsonNodeType GetType() const override { return SJsonNodeType::JSON_NULL; };
        bool IsNull() const override { return true; }
    };

    class SJsonBoolNode : public SJsonNode {
    public:
        SJsonBoolNode(bool value) : SJsonNode(), _value(value) {}
        ~SJsonBoolNode() override {}

        SJsonNodeType GetType() const override { return SJsonNodeType::JSON_BOOL; };
        bool GetBool() const override { return _value; }

    private:
        bool _value;
    };


    class SJsonIntNode : public SJsonNode {
    public:
        SJsonIntNode(ll value) : SJsonNode(), _value(value) {}
        ~SJsonIntNode() override {}

        SJsonNodeType GetType() const override { return SJsonNodeType::JSON_INT; };
        ll GetInt() const override { return _value; }
        double GetFloat() const override { return (double)_value; }

    private:
        ll _value;
    };

    class SJsonFloatNode : public SJsonNode {
    public:
        SJsonFloatNode(double value) : SJsonNode(), _value(value) {}
        ~SJsonFloatNode() override {}

        SJsonNodeType GetType() const override { return SJsonNodeType::JSON_FLOAT; };
        double GetFloat() const override { return _value; }
        ll GetInt() const override { return (ll)_value; }

    private:
        double _value;
    };

    class SJsonStringNode : public SJsonNode {
    public:
        SJsonStringNode(const std::string& value) : SJsonNode(), _value(value) {}
        ~SJsonStringNode() override {}

        SJsonNodeType GetType() const override { return SJsonNodeType::JSON_STRING; };
        std::string GetString() const override { return _value; }

    private:
        std::string _value;
    };




    // 模板便捷操作
    template<typename T>
    T TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, T defValue);

    template<typename T>
    inline T TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, T defValue) {
        throw OperationError("Unknown JSON Value Type");
    }
    template<>
    inline int TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, int defValue) {
        if (node->HasMember(name)) {
            return node->GetMember(name)->GetInt();
        }
        return defValue;
    }
    template<>
    inline ll TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, ll defValue) {
        if (node->HasMember(name)) {
            return node->GetMember(name)->GetInt();
        }
        return defValue;
    }

    template<>
    inline bool TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, bool defValue) {
        if (node->HasMember(name)) {
            return node->GetMember(name)->GetBool();
        }
        return defValue;
    }

    template<>
    inline float TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, float defValue) {
        if (node->HasMember(name)) {
            return node->GetMember(name)->GetFloat();
        }
        return defValue;
    }

    template<>
    inline double TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, double defValue) {
        if (node->HasMember(name)) {
            return node->GetMember(name)->GetFloat();
        }
        return defValue;
    }

    template<>
    inline std::string TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, std::string defValue) {
        if (node->HasMember(name)) {
            return node->GetMember(name)->GetString();
        }
        return defValue;
    }

    std::shared_ptr<SJsonNode> SJsonParse(const std::string& text);
}
