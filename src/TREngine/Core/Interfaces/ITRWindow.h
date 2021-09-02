#pragma once
#include <memory>
#include <TREngine_Interfaces.h>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
class ITRWindow
{
public:
	virtual void Initialize(const EngineSettings& config) = 0;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual bool ShouldClose() const = 0;
	virtual void PollEvents() = 0;

	virtual const IInputController& GetInputController() const = 0;

	virtual glm::ivec2 GetWindowSize() const = 0;
};
TRV2_NAMESPACE_END