#pragma once
#include <memory>
#include <TREngine_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class ITRAPIGenerator
{
public:
	virtual void Initialize(const EngineSettings& config) = 0;
	virtual std::shared_ptr<ITRGraphicsDevice> CreateGraphicsDevice() const = 0;
	virtual std::shared_ptr<ITRWindow> CreateWindow() const = 0;
	virtual std::shared_ptr<ITRAPIUtils> CreateAPIUtils() const = 0;
};
TRV2_NAMESPACE_END