#pragma once
#include <Graphics/Graphics_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class OpenGLGraphicsDevice : public IGraphicsDevice {
public:
	OpenGLGraphicsDevice();
	~OpenGLGraphicsDevice() override;

	void Initialize(const EngineSettings& clientConfig) override;
	std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const override;
private:
};
TRV2_NAMESPACE_END