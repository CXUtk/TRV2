#pragma once
#include <TRGame/TRGame_Interfaces.h>

#include <TREngine/Core.h>
#include <TREngine/Core/Gamplay/Scene.h>
#include <TREngine/Core/Structures/Rect.hpp>


class MapScene : public trv2::Scene
{
public:
	MapScene(trv2::Engine* engine, TRGame* game);
	virtual ~MapScene() override;

	void Update(double deltaTime) override;
	void Draw(double deltaTime) override;

	void FocusOnPlayer();

private:
	TRGame* _game;

	float _expScale = 0.f;
	glm::vec2 _mouseDragStart{};
	glm::vec2 _oldScreenPos{};

	trv2::Rectf _screenRect{};
	glm::mat4 _worldProjection{};
};