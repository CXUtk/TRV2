#pragma once
#include <Graphics/GraphicsInterface/IGraphicsDevice.h>

class OpenGLGraphicsDevice : public IGraphicsDevice {
public:
	OpenGLGraphicsDevice();

	void Initialize() override;
	void Loop() override;

private:
	~OpenGLGraphicsDevice();

	GLFWwindow* _window;
	int a;
};