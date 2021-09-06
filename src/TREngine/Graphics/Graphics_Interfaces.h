#pragma once
#include <string>
#include <vector>
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

enum class ShaderType
{
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER,
    TESSELATION_SHADER,
    COMPUTE_SHADER,

    __COUNT,
};


using IVertexBufferHandle = unsigned int;
using IShaderHandle = unsigned int;
using IShaderProgramHandle = unsigned int;
using ITextureHandle = unsigned int;

class VertexLayout;


/**
 * @brief Graphics device interface. Includes platform independent operations to create and draw rendering objects.
*/
class IGraphicsDevice
{
public:
    IGraphicsDevice(const EngineSettings* clientConfig) {}
    virtual ~IGraphicsDevice() = 0 {};

    /**
     * @brief Set vertex attributes for current binding vertex array object
     * @param layout The vertex layout used for this vertex array buffer
    */
    virtual void SetupVertexAttributes(const VertexLayout& layout) const = 0;

    /**
     * @brief Create a new vertex array object
     * @return The handle to that vertex array
    */
    virtual IVertexBufferHandle CreateVertexArray() const = 0;

    /**
     * @brief Create a new buffer object
     * @return The handle to that vertex buffer
    */
    virtual IVertexBufferHandle CreateBuffer() const = 0;

    /**
     * @brief Create multiple vertex arrays
     * @param size The number of vertex arrays to create
     * @param hOut Handler array to store those objects
    */
    virtual void CreateVertexArrays(int size, IVertexBufferHandle* hOut) const = 0;

    /**
     * @brief Create multiple vertex buffers
     * @param size The number of vertex buffers to create
     * @param hOut Handler array to store those objects
    */
    virtual void CreateBuffers(int size, IVertexBufferHandle* hOut) const = 0;

    /**
     * @brief Bind the buffer object and set its size
     * @param type Type of the buffer object
     * @param handle Handle to the buffer object
     * @param size Size of the buffer
     * @param data Data source to initialize this buffer, nullptr if you don't want to initialize
     * @param bufferHint Hint to this buffer. @see BufferHint
    */
    virtual void SetBufferData(BufferType type, IVertexBufferHandle handle,
        size_t size, const void* data, BufferHint bufferHint) const = 0;

    /**
     * @brief Subsitute the data in given buffer object
     * @param type Type of the buffer object
     * @param handle Handle to the buffer object
     * @param offset Copy starting offset of this action
     * @param size Size of the data
     * @param data Data source that will be copied to this buffer
    */
    virtual void ChangeBufferData(BufferType type, IVertexBufferHandle handle,
        size_t offset, size_t size, const void* data) const = 0;

    /**
     * @brief Bind the buffer to current context
     * @param type Type of the buffer
     * @param handle Handle of the buffer object
    */
    virtual void BindBuffer(BufferType type, IVertexBufferHandle handle) const = 0;

    /**
     * @brief Bind the vertex array object to current context
     * @param handle Handle of the vertex array object
    */
    virtual void BindVertexArray(IVertexBufferHandle handle) const = 0;

    /**
     * @brief Unbind current vertex array object from context
    */
    virtual void UnbindVertexArray() const = 0;

    /**
     * @brief Bind a texture 2d object to current context
     * @param slot Texture slot in graphics device
     * @param textureHandle 
    */
    virtual void BindTexture2DSlot(int slot, const ITexture2D* texture) const = 0;

    /**
     * @brief Get maximum texture slot number in this graphics device
     * @return 
    */
    virtual int GetMaxTextureSlots() const = 0;

    /**
     * @brief Draw primitives using indexed buffer from current context
     * @param type Type of the primitives
     * @param count Number of such primitives
     * @param dataType Data type of indicies
     * @param offset Starting offset of the drawing buffer
    */
    virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset) const = 0;
};

/**
 * @brief Platform indenpendent shader object interface
*/
class IShaderProgram
{
public:
    IShaderProgram(const std::shared_ptr<IRawShader>& vertexShader, const std::shared_ptr<IRawShader>& fragmentShader) {}
    IShaderProgram(const std::vector<std::shared_ptr<IRawShader>>& shaders) {}

    virtual ~IShaderProgram() = 0 {};

    virtual IShaderProgramHandle GetHandle() const = 0;

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
 * @brief Platform indenpendent 2d texture object interface
*/
class ITexture2D 
{
public:
    virtual ~ITexture2D() = 0 {};

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    virtual ITextureHandle GetId() const = 0;
private:
};

/**
 * @brief Compiled shaders, components of final ShaderProgram object
*/
class IRawShader
{
public:
    IRawShader(const char* code, ShaderType shaderType, const char* fileName) {}
    virtual ~IRawShader() = 0 {}

    virtual IShaderHandle GetHandle() const = 0;
    virtual ShaderType GetShaderType() const = 0;
    virtual const char* GetFileName() const = 0;
};
TRV2_NAMESPACE_END