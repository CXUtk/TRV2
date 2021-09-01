#pragma once
#include <Graphics/Interfaces/ITRGraphicsDevice.h>

TRV2_NAMESPACE_BEGIN
class OpenGLGraphicsDevice : public ITRGraphicsDevice {
public:
	OpenGLGraphicsDevice(TREngine* engine);
	~OpenGLGraphicsDevice();

	void Initialize(const EngineSettings& clientConfig) override;
	std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const override;
private:
	TREngine* _engine;
};
TRV2_NAMESPACE_END