#pragma once
#include <string>
#include <glm/glm.hpp>
#include <TREngine_Interfaces.h>

TRV2_NAMESPACE_BEGIN
enum class BufferDataType
{
    FLOAT,
    INT,
    UNSIGNED_INT,
    SHORT,
    UNSIGNED_SHORT,
    BYTE,
    UNSIGNED_BYTE,
    __COUNT
};

enum class BufferHint
{
    STATIC_DRAW,
    DYNAMIC_DRAW,
    __COUNT
};

enum class BufferType
{
    ARRAY_BUFFER,
    INDEX_BUFFER,
    __COUNT
};

enum class PrimitiveType
{
    TRIANGLE_LIST,
    TRIANGLE_STRIP,
    LINE_LIST,
    LINE_STRIP,
    POINTS,
    __COUNT
};


using IGraphicsHandle = unsigned int;
class VertexLayout;


/**
 * @brief Graphics device interface. Includes platform independent operations to create and draw rendering objects.
*/
class IGraphicsDevice
{
public:
    IGraphicsDevice(const EngineSettings* clientConfig) {}
    virtual ~IGraphicsDevice() = 0 {};
    virtual std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const = 0;
    virtual void SetupVertexAttributes(const VertexLayout& layout) const = 0;

    virtual IGraphicsHandle CreateVertexArray() const = 0;
    virtual IGraphicsHandle CreateBuffer() const = 0;

    virtual void CreateVertexArrays(int size, IGraphicsHandle* hOut) const = 0;
    virtual void CreateBuffers(int size, IGraphicsHandle* hOut) const = 0;

    virtual void SetBufferData(BufferType type, IGraphicsHandle handle, 
        size_t size, const void* data, BufferHint bufferHint) const = 0;
    virtual void ChangeBufferData(BufferType type, IGraphicsHandle handle, 
        size_t offset, size_t size, const void* data) const = 0;

    virtual void BindBuffer(BufferType type, IGraphicsHandle handle) const = 0;

    virtual void BindVertexArray(IGraphicsHandle handle) const = 0;
    virtual void UnbindVertexArray() const = 0;

    virtual void BindTexture2DSlot(int slot, IGraphicsHandle textureHandle) const = 0;

    virtual int GetMaxTextureSlots() const = 0;

    virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset) const = 0;
};

/**
 * @brief Platform indenpendent shader object interface
*/
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

/**
 * @brief 
*/
class ISpriteRenderer 
{
public:
    virtual ~ISpriteRenderer() = 0 {};

    virtual void Begin(const glm::mat4& transform) = 0;
    virtual void End() = 0;

    virtual void Draw(glm::vec2 pos, glm::vec2 size,
        glm::vec2 origin, float rotation, const glm::vec4& color) = 0;
    virtual void Draw(const ITexture2D* texture, glm::vec2 pos, glm::vec2 size,
        glm::vec2 origin, float rotation, const glm::vec4& color) = 0;

private:
};

/**
 * @brief Platform indenpendent 2d texture object interface
*/
class ITexture2D 
{
public:
    virtual ~ITexture2D() = 0 {};

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    virtual IGraphicsHandle GetId() const = 0;
private:
};
TRV2_NAMESPACE_END