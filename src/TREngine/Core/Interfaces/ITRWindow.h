#pragma once
#include <memory>
#include <TREngine_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class ITRWindow
{
public:
	virtual void Initialize(const EngineSettings& config) = 0;
	virtual void BeginFrame() = 0;
	virtual bool ShouldClose() const = 0;
	virtual void SwapBuffers() = 0;
	virtual void PollEvents() = 0;
};
TRV2_NAMESPACE_END