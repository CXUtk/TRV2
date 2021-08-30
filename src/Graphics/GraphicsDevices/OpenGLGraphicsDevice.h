#pragma once
#include <Graphics/Interfaces/ITRGraphicsDevice.h>

class OpenGLGraphicsDevice : public ITRGraphicsDevice {
public:
	OpenGLGraphicsDevice();
	~OpenGLGraphicsDevice();

	void Initialize(const ClientConfig* clientConfig) override;
	std::shared_ptr<OpenGLSpriteRenderer> CreateSpriteRenderer() const override;
private:
};