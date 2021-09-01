#pragma once
#include <Graphics/Interfaces/ITRGraphicsDevice.h>

TRV2_NAMESPACE_BEGIN
class OpenGLGraphicsDevice : public ITRGraphicsDevice {
public:
	OpenGLGraphicsDevice();
	~OpenGLGraphicsDevice();

	void Initialize(const EngineSettings& clientConfig) override;
	std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const override;
private:
};
TRV2_NAMESPACE_END