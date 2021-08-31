#pragma once
#include <TREngine/Interfaces.h>
#include <memory>

class ITRGraphicsDevice
{
public:
	virtual void Initialize(const ClientConfig* clientConfig) = 0;
	virtual std::shared_ptr<OpenGLSpriteRenderer> CreateSpriteRenderer() const = 0;
};