#pragma once
#include <TREngine_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class ITexture2D
{
public:
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual unsigned int GetId() const = 0;
	virtual void Bind(int slot) const = 0;
private:
};
TRV2_NAMESPACE_END