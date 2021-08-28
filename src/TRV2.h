#pragma once

// Graphics
class IGraphicsDevice;

// Configs
class ClientConfig;

// Utils
class Logger;

// Macros
#define ReadonlyProperty(type, publicName, privateName) public: type Get##publicName() { return _##privateName; } \
private: type _##privateName;

#define ReadonlyPropertyF(typeF, type, publicName, privateName) public: typeF Get##publicName() { return _##privateName; } \
private: type _##privateName;
