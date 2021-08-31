#pragma once
#include <Interfaces.h>
#include <memory>

class ITRGraphicsDevice
{
public:
	virtual void Initialize(const ClientConfig* clientConfig) = 0;
	virtual std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const = 0;
};