#pragma once


class IGraphicsDevice
{
public:
	virtual void Initialize() = 0;
	virtual void Loop() = 0;
};