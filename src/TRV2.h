#pragma once
#include <memory>

// Graphics
class IGraphicsDevice;

// Configs
class ClientConfig;


template<typename T, typename... _Types>
inline std::shared_ptr<T> TR_make_shared(_Types&&... _Args) {
	return std::shared_ptr<T>(new T(..._Args));
}