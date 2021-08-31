#pragma once
class TREngine;

// Interfaces
// Core
class ITRWindow;

// Assets
class AssetsManager;

// Graphics
class ITRGraphicsDevice;
class ITRGameGraphicsAPIUtils;
class ITRGameGraphicsAPIGenerator;
class ISpriteRenderer;

// Configs
class ClientConfig;

// Utils
class Logger;
struct Rect;
struct RectI;


// Game
class Tile;
class GameWorld;

// Temp
class OpenGLShader;


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