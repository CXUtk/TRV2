#pragma once
#include <string>
#include <glm/glm.hpp>
#include <TREngine_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class IGraphicsDevice
{
public:
    virtual ~IGraphicsDevice() = 0 {};
    virtual void Initialize(const EngineSettings& clientConfig) = 0;
    virtual std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const = 0;
};

class IShader 
{
public:
    virtual ~IShader() = 0 {};
    virtual void Apply() = 0;
    virtual void SetParameteri1(const std::string& name, int value) = 0;
    virtual void SetParameterfv2(const std::string& name, glm::vec2 value) = 0;
    virtual void SetParameterfv4(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetParameterfm4x4(const std::string& name, const glm::mat4& value) = 0;
    virtual void SetParameterfvArray(const std::string& name, const float* data, int size) = 0;
    virtual void SetParameterintvArray(const std::string& name, const int* data, int size) = 0;
private:
};

class ISpriteRenderer 
{
public:
    virtual ~ISpriteRenderer() = 0 {};

    virtual void Begin(const glm::mat4& transform) = 0;
    virtual void End() = 0;

    virtual void Draw(glm::vec2 pos, glm::vec2 size,
        glm::vec2 origin, float rotation, const glm::vec4& color) = 0;
    virtual void Draw(const ITexture2D& texture, glm::vec2 pos, glm::vec2 size,
        glm::vec2 origin, float rotation, const glm::vec4& color) = 0;

private:
};

class ITexture2D 
{
public:
    virtual ~ITexture2D() = 0 {};

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    virtual unsigned int GetId() const = 0;
    virtual void Bind(int slot) const = 0;
private:
};
TRV2_NAMESPACE_END