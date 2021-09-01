#pragma once
#include <memory>
#include <TREngine_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class ITRAPIUtils
{
public:
	virtual double GetTime() = 0;
};
TRV2_NAMESPACE_END