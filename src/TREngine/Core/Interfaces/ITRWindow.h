#pragma once
#include <memory>
#include <TREngine/Interfaces.h>

class ITRWindow
{
public:
	virtual void Initialize(const ClientConfig* config) = 0;
	virtual void BeginFrame() = 0;
	virtual bool ShouldClose() const = 0;
	virtual void SwapBuffers() = 0;
	virtual void PollEvents() = 0;
};