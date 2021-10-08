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

class InputController;
class AssetsManager;
class SpriteRenderer;
class EngineSettings;

class RawShader;
class ShaderProgram;
class Texture2D;
class RenderTarget2D;
class UniversalRenderer;

class Scene;


// GraphicsAPI
class IGraphicsDevice;
class IGraphicsResourceManager;
class IGraphicsProvider;


// Platform
class IGameWindow;
class IFileSystem;
class IPlatformProvider;
class GameTimer;


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
inline T& ref(const std::shared_ptr<T>& ptr) { return static_cast<T&>(*ptr); }

/**
 * @brief Converts a shared pointer to a const reference of object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T> 
inline const T& cref(const std::shared_ptr<T>& ptr) { return static_cast<const T&>(*ptr); }

/**
 * @brief Converts a shared pointer to a pointer of the object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline T* ptr(const std::shared_ptr<T>& ptr) { return static_cast<T*>(ptr.get()); }

/**
 * @brief Converts a shared pointer to a pointer of the object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline const T* cptr(const std::shared_ptr<T>& ptr) { return static_cast<const T*>(ptr.get()); }

/**
 * @brief Converts a unique pointer to a pointer of the object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline T* ptr(const std::unique_ptr<T>& ptr) { return static_cast<T*>(ptr.get()); }


/**
 * @brief Converts a unique pointer to a const pointer of object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline const T* cptr(const std::unique_ptr<T>& ptr) { return static_cast<const T*>(ptr.get()); }

TRV2_NAMESPACE_END


constexpr float OneMinusEpsilon = 0.99999994f;
using byte8 = unsigned char;
using uint = unsigned int;