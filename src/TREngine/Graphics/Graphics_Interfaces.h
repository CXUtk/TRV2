#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <Core.h>

TRV2_NAMESPACE_BEGIN

enum class EngineDataType
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

	__COUNT
};


enum class PixelFormat
{
	RED,
	RG,
	RGB,
	RGBA,
	RGB16F,
	DEPTH,
	STENCIL,
	DEPTH_STENCIL,

	__COUNT
};

enum class TextureSampleMethod
{
	NEAREST,
	BI_LINEAR,
	BI_LINEAR_MIPMAP,

	__COUNT
};

enum class TextureWarpMethod
{
	REPEAT,
	CLAMP_TO_EDGE,
	MIRRORED_REPEAT,

	__COUNT
};

enum class BlendingMode
{
	None,
	AlphaBlend,
	Additive,

	__COUNT
};

enum class DepthTestingMode
{
	None,
	DepthTest,
	DepthTestNoApply,

	__COUNT
};

enum class CullingMode
{
	None,
	CullCCW,
	CullCW,

	__COUNT
};

enum class PolygonMode
{
	WIREFRAME,
	FILL,

	__COUNT
};


enum class DepthTestingFunction
{
	ALWAYS,
	NEVER,
	LESS,
	EQUAL,
	LEQUAL,
	GREATER,
	NEQUAL,
	GEQUAL,

	__COUNT
};



// Classes
class VertexLayout;

struct TextureParameters
{
	TextureSampleMethod SampleMethod = TextureSampleMethod::NEAREST;
	TextureWarpMethod WarpMethod = TextureWarpMethod::REPEAT;
	PixelFormat InternalFormat = PixelFormat::RGB;

	TextureParameters() {}
};

struct byte_color
{
	byte8 r, g, b;
};


// Handles
using IVertexBufferHandle = unsigned int;
using IShaderHandle = unsigned int;
using IShaderProgramHandle = unsigned int;
using ITextureHandle = unsigned int;
using IRenderTarget2DHandle = unsigned int;

class IGraphicsResource
{
public:
	IGraphicsResource(IGraphicsResourceManager* resourceManager) 
		: _resourceManager(resourceManager) {}
	virtual ~IGraphicsResource() = 0 {}

protected:
	IGraphicsResourceManager* _resourceManager;
};

class IGraphicsProvider
{
public:
	virtual ~IGraphicsProvider() = 0 {};

	virtual IGraphicsDevice* GetCurrentDeivce() const = 0;
	virtual IGraphicsResourceManager* GetGraphicsResourceManager() const = 0;
};

/**
 * @brief Graphics device interface. Includes platform independent operations to create and draw rendering objects.
*/
class IGraphicsDevice
{
public:
	virtual ~IGraphicsDevice() = 0 {};

	/**
	 * @brief Set vertex attributes for current binding vertex array object
	 * @param layout The vertex layout used for this vertex array buffer
	*/
	virtual void SetupVertexAttributes(const VertexLayout& layout) = 0;

	/**
	 * @brief Create a new vertex array object
	 * @return The handle to that vertex array
	*/
	virtual IVertexBufferHandle CreateVertexArray() = 0;

	/**
	 * @brief Create a new buffer object
	 * @return The handle to that vertex buffer
	*/
	virtual IVertexBufferHandle CreateBuffer() = 0;

	/**
	 * @brief Create multiple vertex arrays
	 * @param size The number of vertex arrays to create
	 * @param hOut Handler array to store those objects
	*/
	virtual void CreateVertexArrays(int size, IVertexBufferHandle* hOut) = 0;

	/**
	 * @brief Create multiple vertex buffers
	 * @param size The number of vertex buffers to create
	 * @param hOut Handler array to store those objects
	*/
	virtual void CreateBuffers(int size, IVertexBufferHandle* hOut) = 0;

	/**
	 * @brief Bind the buffer object and set its size
	 * @param type Type of the buffer object
	 * @param handle Handle to the buffer object
	 * @param size Size of the buffer
	 * @param data Data source to initialize this buffer, nullptr if you don't want to initialize
	 * @param bufferHint Hint to this buffer. @see BufferHint
	*/
	virtual void SetBufferData(BufferType type, IVertexBufferHandle handle,
		size_t size, const void* data, BufferHint bufferHint) = 0;

	/**
	 * @brief Subsitute the data in given buffer object
	 * @param type Type of the buffer object
	 * @param handle Handle to the buffer object
	 * @param offset Copy starting offset of this action
	 * @param size Size of the data
	 * @param data Data source that will be copied to this buffer
	*/
	virtual void ChangeBufferData(BufferType type, IVertexBufferHandle handle,
		size_t offset, size_t size, const void* data) = 0;

	/**
	 * @brief Bind the buffer to current context
	 * @param type Type of the buffer
	 * @param handle Handle of the buffer object
	*/
	virtual void BindBuffer(BufferType type, IVertexBufferHandle handle) = 0;

	/**
	 * @brief Bind the vertex array object to current context
	 * @param handle Handle of the vertex array object
	*/
	virtual void BindVertexArray(IVertexBufferHandle handle) = 0;

	/**
	 * @brief Unbind current vertex array object from context
	*/
	virtual void UnbindVertexArray() = 0;

	/**
	 * @brief Bind a texture 2d object to current context
	 * @param slot Texture slot in graphics device
	 * @param textureHandle
	*/
	virtual void BindTexture2DSlot(int slot, const Texture2D* texture) = 0;

	/**
	 * @brief Get maximum texture slot number in this graphics device
	 * @return
	*/
	virtual int GetMaxTextureSlots() = 0;

	/**
	 * @brief Draw primitives using indexed buffer from current context
	 * @param type Type of the primitives
	 * @param count Number of such primitives
	 * @param dataType Data type of indicies
	 * @param offset Starting offset of the drawing buffer
	*/
	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, EngineDataType dataType, size_t offset) = 0;

	virtual void DrawPrimitives(PrimitiveType type, size_t count, size_t offset) = 0;

	virtual void SwitchRenderTarget(const RenderTarget2D* renderTarget) = 0;
	virtual void SetViewPort(int x, int y, int width, int height) = 0;
	virtual void Clear(const glm::vec4& color) = 0;

	virtual void UseShader(const ShaderProgram* shader) = 0;

	virtual void SetBlendingMode(BlendingMode mode) = 0;
	virtual void SetDepthTestingMode(DepthTestingMode mode, DepthTestingFunction func) = 0;
	virtual void SetCullingMode(CullingMode mode) = 0;
	virtual void SetPolygonMode(PolygonMode mode) = 0;

	virtual byte_color ReadPixelFromTexture(const Texture2D* texture, int x, int y) = 0;
};


class IGraphicsResourceManager
{
public:
	virtual ~IGraphicsResourceManager() = 0 {};

	virtual ITextureHandle CreateTexture2D(glm::ivec2 size,
	const TextureParameters& parameters, PixelFormat srcFormat,
		EngineDataType dataType, const void* data) = 0;
	virtual void DeleteTexture2D(ITextureHandle handle) = 0;
	virtual void ChangeTexture2D(ITextureHandle handle, glm::ivec2 size,
		const TextureParameters& parameters, PixelFormat srcFormat,
		EngineDataType dataType, const void* data) = 0;

	virtual IShaderHandle CreateRawShader(const char* code, ShaderType shaderType, const char* fileName) = 0;
	virtual void DeleteRawShader(IShaderHandle handle) = 0;

	virtual IShaderProgramHandle CreateShaderProgram(const std::vector<const RawShader*>& shaders) = 0;
	virtual void DeleteShaderProgram(IShaderProgramHandle handle) = 0;

	virtual IRenderTarget2DHandle CreateRenderTarget2D(Texture2D* receiver, glm::ivec2 size) = 0;
	virtual void DeleteRenderTarget2D(IRenderTarget2DHandle handle) = 0;


	virtual void SetShaderParameterMat4x4(ShaderProgram* shader, 
		const std::string& name, const glm::mat4& mat, bool normalized) = 0;
	virtual void SetShaderParameter1i(ShaderProgram* shader, const std::string& name, int value) = 0;
};
TRV2_NAMESPACE_END