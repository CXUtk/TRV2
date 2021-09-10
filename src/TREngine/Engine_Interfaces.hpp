#pragma once
#include <memory>
#include <cassert>

#define TRV2_NAMESPACE_BEGIN namespace trv2 {
#define TRV2_NAMESPACE_END }

TRV2_NAMESPACE_BEGIN
// Interfaces
// Core
class Engine;
class Application;


// GraphicsAPI
class IGraphicsDevice;
class IRawShader;
class IShaderProgram;
class ITexture2D;
class IRenderTarget2D;

class IGraphicsProvider;



// Platform
class IGameWindow;
class IInputController;

class IPlatformProvider;



// Assets
class AssetsManager;
class SpriteRenderer;

// Configs
class EngineSettings;

// Utils
class Logger;

// Templates
/**
 * @brief Converts a shared pointer to a reference of object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return reference to the object
*/
template<typename T> 
T& ref(const std::shared_ptr<T>& ptr) { return static_cast<T&>(*ptr); }

/**
 * @brief Converts a shared pointer to a const reference of object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T> 
const T& cref(const std::shared_ptr<T>& ptr) { return static_cast<const T&>(*ptr); }

/**
 * @brief Converts a shared pointer to a pointer of the object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
T* ptr(const std::shared_ptr<T>& ptr) { return static_cast<T*>(ptr.get()); }


/**
 * @brief Converts a shared pointer to a const pointer of object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
const T* cptr(const std::shared_ptr<T>& ptr) { return static_cast<const T*>(ptr.get()); }


// Enums
enum class GraphicsAPIType {
	None,
	OpenGL,
	DirectX
};

enum class PlatformType
{
	GLFW,
	WINDOWS,
};



// Global Constants

constexpr float OneMinusEpsilon = 0.99999994;

TRV2_NAMESPACE_END


// Macros
#define ReadonlyProperty(type, publicName, privateName) public: type Get##publicName() const { return _##privateName; } \
private: type _##privateName

#define ReadonlyPropertyF(typeF, type, publicName, privateName) public: typeF Get##publicName() const { return _##privateName; } \
private: type _##privateName


#define ReadonlySharedPtrProperty(type, publicName, privateName) public: type* Get##publicName() { return _##privateName.get(); } \
private: std::shared_ptr<type> _##privateName

// 类似于Get访问器，不过可以修改对象内容
#define ReadonlyUniquePtrProperty(type, publicName, privateName) public: type* Get##publicName() { return _##privateName.get(); } \
private: std::unique_ptr<type> _##privateName

// 类似于Get访问器，保证是const引用，不能修改内容
#define ReadonlyConstSharedPtrProperty(type, publicName, privateName) public: const type* Get##publicName() const { return _##privateName.get(); } \
private: std::shared_ptr<type> _##privateName

// 类似于Get访问器的引用形式，不过可以修改对象内容
#define ReadonlyReferenceProperty(type, publicName, privateName) public: type& Get##publicName() { return *(_##privateName.get()); } \
private: std::shared_ptr<type> _##privateName

// 类似于Get访问器的引用形式，保证是const引用，不能修改内容
#define ReadonlyConstReferenceProperty(type, publicName, privateName) public: const type& Get##publicName() { return trv2::cref(_##privateName); } \
private: std::shared_ptr<type> _##privateName