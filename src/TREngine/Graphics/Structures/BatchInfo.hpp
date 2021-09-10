#pragma once
#include <Core.hpp>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
struct BatchInfo
{
	glm::vec2 Position, Size;
	glm::vec4 Color;
	BatchInfo() = default;
	BatchInfo(glm::vec2 pos, glm::vec2 size, const glm::vec4& color) : Position(pos), Size(size), Color(color) {}
};

enum class SpriteSortMode
{
	Deferred,
	FrontToBack,
	BackToFront,
	Texture,
};

struct BatchSettings
{
	SpriteSortMode SpriteSortMode;
	IShaderProgram* Shader;

	BatchSettings() : SpriteSortMode(SpriteSortMode::Deferred), Shader(nullptr) {}
};

struct BatchState
{
	glm::mat4 WorldTransform;
	bool IsBatchBegin;
	BatchSettings Settings;
	BatchState() = default;
};

struct BYTE_Color
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};

inline BYTE_Color vec4ToByteColor(const glm::vec4& color)
{
	BYTE_Color C;
	C.R = (unsigned char)(std::min(OneMinusEpsilon, color.r) * 256);
	C.G = (unsigned char)(std::min(OneMinusEpsilon, color.g) * 256);
	C.B = (unsigned char)(std::min(OneMinusEpsilon, color.b) * 256);
	C.A = (unsigned char)(std::min(OneMinusEpsilon, color.a) * 256);
	return C;
}

struct BatchVertex2D
{
	glm::vec2 Position;
	glm::vec2 TextureCoords;
	BYTE_Color Color;
	float TextureIndex;

	BatchVertex2D() = default;
	BatchVertex2D(glm::vec2 pos, glm::vec2 texCoords, const glm::vec4& color) : Position(pos),
		TextureCoords(texCoords), TextureIndex(0.f) {
		Color = vec4ToByteColor(color);
	}
};
TRV2_NAMESPACE_END