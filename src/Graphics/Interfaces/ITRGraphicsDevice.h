#pragma once
#include <TRV2.h>
#include <memory>

class ITRGraphicsDevice
{
public:
	virtual void Initialize(const ClientConfig* clientConfig) = 0;
};