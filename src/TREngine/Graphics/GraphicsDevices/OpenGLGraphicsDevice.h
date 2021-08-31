#pragma once
#include <Graphics/Interfaces/ITRGraphicsDevice.h>

class OpenGLGraphicsDevice : public ITRGraphicsDevice {
public:
	OpenGLGraphicsDevice(TREngine* engine);
	~OpenGLGraphicsDevice();

	void Initialize(const ClientConfig* clientConfig) override;
	std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const override;
private:
	TREngine* _engine;
};