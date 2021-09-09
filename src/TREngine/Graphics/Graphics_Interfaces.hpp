#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <TREngine_Interfaces.hpp>

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


// Classes
class VertexLayout;

struct TextureParameters
{
	TextureSampleMethod SampleMethod;
	TextureWarpMethod TextureWarpMethod;

	TextureParameters() : SampleMethod(TextureSampleMethod::NEAREST), TextureWarpMethod(TextureWarpMethod::REPEAT) {}
};



///**
// * @brief Graphics device interface. Includes platform independent operations to create and draw rendering objects.
//*/
//template<typename _API>
//class _GraphicsDevice_Base
//{
//public:
//	using ITexture2D = typename _API::_Texture2D_Type;
//
//	virtual ~_GraphicsDevice_Base() = 0 {};
//
//	/**
//	 * @brief Set vertex attributes for current binding vertex array object
//	 * @param layout The vertex layout used for this vertex array buffer
//	*/
//	virtual void SetupVertexAttributes(const VertexLayout& layout) = 0;
//
//	/**
//	 * @brief Create a new vertex array object
//	 * @return The handle to that vertex array
//	*/
//	virtual IVertexBufferHandle CreateVertexArray() = 0;
//
//	/**
//	 * @brief Create a new buffer object
//	 * @return The handle to that vertex buffer
//	*/
//	virtual IVertexBufferHandle CreateBuffer() = 0;
//
//	/**
//	 * @brief Create multiple vertex arrays
//	 * @param size The number of vertex arrays to create
//	 * @param hOut Handler array to store those objects
//	*/
//	virtual void CreateVertexArrays(int size, IVertexBufferHandle* hOut) = 0;
//
//	/**
//	 * @brief Create multiple vertex buffers
//	 * @param size The number of vertex buffers to create
//	 * @param hOut Handler array to store those objects
//	*/
//	virtual void CreateBuffers(int size, IVertexBufferHandle* hOut) = 0;
//
//	/**
//	 * @brief Bind the buffer object and set its size
//	 * @param type Type of the buffer object
//	 * @param handle Handle to the buffer object
//	 * @param size Size of the buffer
//	 * @param data Data source to initialize this buffer, nullptr if you don't want to initialize
//	 * @param bufferHint Hint to this buffer. @see BufferHint
//	*/
//	virtual void SetBufferData(BufferType type, IVertexBufferHandle handle,
//		size_t size, const void* data, BufferHint bufferHint) = 0;
//
//	/**
//	 * @brief Subsitute the data in given buffer object
//	 * @param type Type of the buffer object
//	 * @param handle Handle to the buffer object
//	 * @param offset Copy starting offset of this action
//	 * @param size Size of the data
//	 * @param data Data source that will be copied to this buffer
//	*/
//	virtual void ChangeBufferData(BufferType type, IVertexBufferHandle handle,
//		size_t offset, size_t size, const void* data) = 0;
//
//	/**
//	 * @brief Bind the buffer to current context
//	 * @param type Type of the buffer
//	 * @param handle Handle of the buffer object
//	*/
//	virtual void BindBuffer(BufferType type, IVertexBufferHandle handle) = 0;
//
//	/**
//	 * @brief Bind the vertex array object to current context
//	 * @param handle Handle of the vertex array object
//	*/
//	virtual void BindVertexArray(IVertexBufferHandle handle) = 0;
//
//	/**
//	 * @brief Unbind current vertex array object from context
//	*/
//	virtual void UnbindVertexArray() = 0;
//
//	/**
//	 * @brief Bind a texture 2d object to current context
//	 * @param slot Texture slot in graphics device
//	 * @param textureHandle
//	*/
//	virtual void BindTexture2DSlot(int slot, const ITexture2D* texture) = 0;
//
//	/**
//	 * @brief Get maximum texture slot number in this graphics device
//	 * @return
//	*/
//	virtual int GetMaxTextureSlots() = 0;
//
//	/**
//	 * @brief Draw primitives using indexed buffer from current context
//	 * @param type Type of the primitives
//	 * @param count Number of such primitives
//	 * @param dataType Data type of indicies
//	 * @param offset Starting offset of the drawing buffer
//	*/
//	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset) = 0;
//};
//
///**
// * @brief Platform indenpendent shader object interface
//*/
//template<typename _API>
//class _ShaderProgram_Base
//{
//public:
//	using RawShader = typename _API::_RawShader_Type;
//	_ShaderProgram_Base(const std::shared_ptr<RawShader>& vertexShader, const std::shared_ptr<RawShader>& fragmentShader) {}
//	_ShaderProgram_Base(const std::vector<std::shared_ptr<RawShader>>& shaders) {}
//
//	virtual ~_ShaderProgram_Base() = 0 {};
//
//	virtual IShaderProgramHandle GetHandle() const = 0;
//
//	virtual void Apply() = 0;
//	virtual void SetParameteri1(const std::string& name, int value) = 0;
//	virtual void SetParameterfv2(const std::string& name, glm::vec2 value) = 0;
//	virtual void SetParameterfv4(const std::string& name, const glm::vec4& value) = 0;
//	virtual void SetParameterfm4x4(const std::string& name, const glm::mat4& value) = 0;
//	virtual void SetParameterfvArray(const std::string& name, const float* data, int size) = 0;
//	virtual void SetParameterintvArray(const std::string& name, const int* data, int size) = 0;
//private:
//};
//
//
//
///**
// * @brief Platform indenpendent 2d texture object interface
//*/
//template<typename _API>
//class _Texture2D_Base
//{
//public:
//	using IGraphicsDevice = typename _API::_GraphicsDevice_Type;
//
//	virtual ~_Texture2D_Base() = 0 {};
//
//	virtual int GetWidth() const = 0;
//	virtual int GetHeight() const = 0;
//
//	virtual ITextureHandle GetHandle() const = 0;
//private:
//};
//
///**
// * @brief Compiled shaders, components of final ShaderProgram object
//*/
//template<typename _API>
//class _RawShader_Base
//{
//public:
//	using IGraphicsDevice = typename _API::_GraphicsDevice_Type;
//
//	_RawShader_Base(const char* code, ShaderType shaderType, const char* fileName) {}
//	virtual ~_RawShader_Base() = 0 {}
//
//	virtual IShaderHandle GetHandle() const = 0;
//	virtual ShaderType GetShaderType() const = 0;
//	virtual const char* GetFileName() const = 0;
//};
TRV2_NAMESPACE_END