#pragma once
#include <memory>
#include <TRV2.h>

class IGraphicsDevice
{
public:
	virtual void Initialize() = 0;
	virtual void Loop() = 0;
};