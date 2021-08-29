﻿#pragma once
#include <memory>
#include <TRV2.h>

class ITRGameGraphicsAPIGenerator
{
public:
	virtual void Initialize(const ClientConfig* config) = 0;
	virtual std::shared_ptr<ITRGraphicsDevice> GetGraphicsDevice() const = 0;
	virtual std::shared_ptr<ITRWindow> GetWindow() const = 0;
	virtual std::shared_ptr<ITRGameGraphicsAPIUtils> GetGraphicsAPIUtils() const = 0;
};