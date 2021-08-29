#pragma once
#include <memory>
#include <TRV2.h>
#include <Graphics/Interfaces/ITRGameGraphicsAPI.h>
#include <Graphics/Interfaces/ITRGameGraphicsAPIUtils.h>

class OpenGLTRGameGraphicsAPI : public ITRGameGraphicsAPI
{
public:
	OpenGLTRGameGraphicsAPI();
	~OpenGLTRGameGraphicsAPI();

	void Initialize(const ClientConfig* config) override;
	virtual std::shared_ptr<ITRGraphicsDevice> GetGraphicsDevice() const override {
		return _graphicsDevice;
	}
	virtual std::shared_ptr<ITRWindow> GetWindow() const override {
		return _gameWindow;
	}
	virtual std::shared_ptr<ITRGameGraphicsAPIUtils> GetGraphicsAPIUtils() const override {
		return _graphicsAPIUtils;
	}

private:
	std::shared_ptr<ITRGraphicsDevice> _graphicsDevice;
	std::shared_ptr<ITRWindow> _gameWindow;
	std::shared_ptr<ITRGameGraphicsAPIUtils> _graphicsAPIUtils;
};

class OpenGLTRGameGraphicsAPIUtils : public ITRGameGraphicsAPIUtils
{
public:
	OpenGLTRGameGraphicsAPIUtils();

	double GetTime() override;
};