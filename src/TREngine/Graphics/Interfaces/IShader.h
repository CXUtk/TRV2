#pragma once
#include <string>
#include <glm/glm.hpp>
class IShader
{
public:
    virtual void Apply() = 0;    
    virtual void SetParameteri1(const std::string& name, int value) = 0;
    virtual void SetParameterfv2(const std::string& name, glm::vec2 value) = 0;
    virtual void SetParameterfv4(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetParameterfm4x4(const std::string& name, const glm::mat4& value) = 0;
private:
};