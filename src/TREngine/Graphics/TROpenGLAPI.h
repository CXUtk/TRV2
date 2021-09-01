#pragma once
#include <memory>
#include <TREngine_Interfaces.h>
#include <Graphics/Interfaces/ITRAPIGenerator.h>
#include <Graphics/Interfaces/ITRAPIUtils.h>

TRV2_NAMESPACE_BEGIN
class TROpenGLAPIGenerator : public ITRAPIGenerator
{
public:
	TROpenGLAPIGenerator();
	~TROpenGLAPIGenerator();

	void Initialize(const EngineSettings& config) override;
	std::shared_ptr<ITRGraphicsDevice> CreateGraphicsDevice() const override {
		return _graphicsDevice;
	}
	std::shared_ptr<ITRWindow> CreateWindow() const override {
		return _gameWindow;
	}
	std::shared_ptr<ITRAPIUtils> CreateAPIUtils() const override {
		return _graphicsAPIUtils;
	}

private:
	std::shared_ptr<ITRGraphicsDevice> _graphicsDevice;
	std::shared_ptr<ITRWindow> _gameWindow;
	std::shared_ptr<ITRAPIUtils> _graphicsAPIUtils;
};

class TROpenGLAPIUtils : public ITRAPIUtils
{
public:
	TROpenGLAPIUtils();

	double GetTime() override;
};
TRV2_NAMESPACE_END