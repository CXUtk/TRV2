#pragma once
#include <glm/glm.hpp>
#include <TRGame/TRGame_Interfaces.h>

#include <TREngine/Core.h>
#include <TREngine/Core/Gamplay/Scene.h>
#include <TREngine/Core/Structures/Rect.hpp>


class MainGameScene: public trv2::Scene
{
public:
	MainGameScene(trv2::Engine * engine, TRGame * game);
	virtual ~MainGameScene() override;

	void Update(double deltaTime) override;
	void Draw(double deltaTime) override;

private:
	void drawTiles();
	void drawShadowMaps();
	void drawPlayers();

	TRGame* _game;

	trv2::RectI _screenRect{};
	trv2::RectI _tileRect{};

	glm::mat4 _worldProjection{};
	glm::mat4 _screenProjection{};

	std::shared_ptr<trv2::RenderTarget2D> _tileTarget;
	std::shared_ptr<trv2::RenderTarget2D> _shadowMap;

	// Shadow map components: lighting
	std::shared_ptr<trv2::RenderTarget2D> _prevShadowMap;
	trv2::RectI _prevTileRect{};

	std::shared_ptr<trv2::RenderTarget2D> _shadowMapSwap[2];
};