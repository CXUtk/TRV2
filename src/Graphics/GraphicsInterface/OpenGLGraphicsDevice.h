#pragma once
#include <Graphics/GraphicsInterface/IGraphicsDevice.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLGraphicsDevice : public IGraphicsDevice {
public:
	OpenGLGraphicsDevice(const std::shared_ptr<ClientConfig>& clientConfig);
	~OpenGLGraphicsDevice();

	void Initialize() override;
	void Loop() override;

private:
	GLFWwindow* _window;
	std::shared_ptr<ClientConfig> _clientConfig;
};