#pragma once
#include <memory>

#define TRV2_NAMESPACE_BEGIN namespace trv2\
{

#define TRV2_NAMESPACE_END }



TRV2_NAMESPACE_BEGIN
class TREngine;

// Interfaces
// Core
class ITRWindow;

// Assets
class AssetsManager;

// Graphics
class ITRGraphicsDevice;
class ITRAPIUtils;
class ITRAPIGenerator;
class ISpriteRenderer;
class IShader;

// Configs
class EngineSettings;

// Utils
class Logger;
struct Rect;
struct RectI;


// Templates
template<typename T>
T& ref(const std::shared_ptr<T>& ptr) { return static_cast<T&>(*ptr); }

template<typename T>
const T& cref(const std::shared_ptr<T>& ptr) { return static_cast<const T&>(*ptr); }
TRV2_NAMESPACE_END


// Macros
#define ReadonlyProperty(type, publicName, privateName) public: type Get##publicName() const { return _##privateName; } \
private: type _##privateName

#define ReadonlyPropertyF(typeF, type, publicName, privateName) public: typeF Get##publicName() const { return _##privateName; } \
private: type _##privateName

// 类似于Get访问器，不过可以修改对象内容
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