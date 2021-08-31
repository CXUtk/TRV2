#pragma once
#include <Interfaces.h>
#include <memory>
#include <Graphics/Shaders/OpenGLShader.h>

class ISpriteRenderer
{
public:
	virtual void Begin(const glm::mat4& transform) = 0;
	virtual void End() = 0;

	virtual void Draw(glm::vec2 pos, glm::vec2 size,
		glm::vec2 origin, float rotation, const glm::vec4& color) = 0;

private:
};