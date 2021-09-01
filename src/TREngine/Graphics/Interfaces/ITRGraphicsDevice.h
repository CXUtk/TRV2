#pragma once
#include <TREngine_Interfaces.h>
#include <memory>

TRV2_NAMESPACE_BEGIN
class ITRGraphicsDevice
{
public:
	virtual void Initialize(const EngineSettings& clientConfig) = 0;
	virtual std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const = 0;
};
TRV2_NAMESPACE_END