#pragma once
#include <memory>
#include <TREngine_Interfaces.h>
#include <Graphics/Interfaces/ITRAPIGenerator.h>
#include <Graphics/Interfaces/ITRAPIUtils.h>

class OpenGLTRGameGraphicsAPIGenerator : public ITRAPIGenerator
{
public:
	OpenGLTRGameGraphicsAPIGenerator(TREngine* engine);
	~OpenGLTRGameGraphicsAPIGenerator();

	void Initialize(const ClientConfig* config) override;
	std::shared_ptr<ITRGraphicsDevice> GetGraphicsDevice() const override {
		return _graphicsDevice;
	}
	std::shared_ptr<ITRWindow> GetWindow() const override {
		return _gameWindow;
	}
	std::shared_ptr<ITRGameGraphicsAPIUtils> GetGraphicsAPIUtils() const override {
		return _graphicsAPIUtils;
	}

private:
	std::shared_ptr<ITRGraphicsDevice> _graphicsDevice;
	std::shared_ptr<ITRWindow> _gameWindow;
	std::shared_ptr<ITRGameGraphicsAPIUtils> _graphicsAPIUtils;

	TREngine* _engine;
};

class OpenGLTRGameGraphicsAPIUtils : public ITRAPIUtils
{
public:
	OpenGLTRGameGraphicsAPIUtils();

	double GetTime() override;
};