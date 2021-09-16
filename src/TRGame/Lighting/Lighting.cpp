#include "Lighting.h"
#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Worlds/GameWorld.hpp>

Lighting::Lighting()
{}

Lighting::~Lighting()
{}

struct LightNode
{
	glm::ivec2 Pos;
	glm::vec3 Lumin;
};

void Lighting::CalculateLight()
{


}

float Lighting::GetLight(glm::ivec2 pos)
{
	auto player = TRGame::GetInstance()->GetLocalPlayer();

	glm::ivec2 lightTile = GameWorld::GetUpperWorldCoord(player->GetPlayerHitbox().BottomLeft());
	glm::ivec2 diff = pos - lightTile;

	double dist = std::sqrt((double)diff.x * diff.x + (double)diff.y * diff.y);
	if (dist > 16) return 0;
	return glm::smoothstep(0.f, 1.f, 1.f / (1.f + (float)dist));
}
