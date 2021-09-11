#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <Core.h>

TRV2_NAMESPACE_BEGIN

class IGameWindow
{
public:
	virtual ~IGameWindow() = 0 {};

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual bool ShouldClose() const = 0;
	virtual void PollEvents() = 0;

	virtual glm::ivec2 GetWindowSize() const = 0;
};

TRV2_NAMESPACE_END