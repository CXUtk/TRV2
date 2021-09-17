#include "Lighting.h"
#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Worlds/GameWorld.hpp>

#include <algorithm>
#include <vector>
#include <queue>

#include <TREngine/Core/Render/SpriteRenderer.h>

Lighting::Lighting()
{}

Lighting::~Lighting()
{}


struct LightNode
{
	glm::ivec2 Pos;
	float Dist;

	bool operator<(const LightNode& B) const
	{
		return this->Dist > B.Dist;
	}
};

int dX[8] = { 1, 1, 1, -1, -1, -1, 0, 0 };
int dY[8] = { 1, 0, -1, 1, 0, -1, 1, -1 };
float distA[8] = { 1.4142f, 1.f, 1.4142f, 1.4142f, 1.f, 1.4142f, 1.f, 1.f };
float distArray[100 * 100];
bool visArray[100 * 100];

void Lighting::CalculateLight(trv2::SpriteRenderer* renderer, const glm::mat4& projection, 
	const GameWorld* world, const trv2::Rect2D<float>& screenRect)
{
	glm::ivec2 botLeft = GameWorld::GetLowerWorldCoord(screenRect.BottomLeft());
	botLeft.x = std::max(0, std::min(world->GetTileMaxX() - 1, botLeft.x));
	botLeft.y = std::max(0, std::min(world->GetTileMaxY() - 1, botLeft.y));



	glm::ivec2 topRight = GameWorld::GetUpperWorldCoord(screenRect.TopRight());
	topRight.x = std::max(0, std::min(world->GetTileMaxX() - 1, topRight.x));
	topRight.y = std::max(0, std::min(world->GetTileMaxY() - 1, topRight.y));

	trv2::Rect2D<int> viewRect(botLeft, topRight - botLeft);

	auto player = TRGame::GetInstance()->GetLocalPlayer();


	for (int i = 0; i < viewRect.Size.x * viewRect.Size.y; i++)
	{
		distArray[i] = 16.f;
		visArray[i] = false;
	}

	glm::ivec2 lightTile = GameWorld::GetUpperWorldCoord(player->GetPlayerHitbox().BottomLeft());

	auto getId = [&viewRect](glm::ivec2 pos) {
		int x = pos.x - viewRect.Position.x;
		int y = pos.y - viewRect.Position.y;
		return y * viewRect.Size.x + x;
	};

	auto valid = [&viewRect, world](glm::ivec2 pos, bool isSolid) {
		if (pos.x < viewRect.Position.x || pos.x >= viewRect.Position.x + viewRect.Size.x 
			|| pos.y < viewRect.Position.y || pos.y >= viewRect.Position.y+ viewRect.Size.y) return false;
		const auto& tile = world->GetTile(pos.x, pos.y);
		if (tile.IsEmpty())return true;
		if (isSolid) return false;
		return true;
	};

	std::priority_queue<LightNode> Q;
	Q.push({ lightTile, 0.f });
	int id = getId(lightTile);
	distArray[id] = 0.f;



	while (!Q.empty())
	{
		auto node = Q.top();
		Q.pop();

		int id = getId(node.Pos);
		if (visArray[id]) continue;
		visArray[id] = true;

		bool solid = world->GetTile(node.Pos.x, node.Pos.y).IsSolid();
		for (int i = 0; i < 8; i++)
		{
			glm::ivec2 nxtPos(node.Pos.x + dX[i], node.Pos.y + dY[i]);
			if (valid(nxtPos, solid))
			{
				int nxtId = getId(nxtPos);
				if (distArray[nxtId] > distA[i] + distArray[id])
				{
					distArray[nxtId] = distA[i] + distArray[id];
					Q.push({ nxtPos, distArray[nxtId] });
				}
			}
		}
	}
	trv2::BatchSettings setting{};
	renderer->Begin(projection, setting);
	{
		auto start = glm::vec2(viewRect.Position);
		for (int i = 0; i < viewRect.Size.x; i++)
		{
			for (int j = 0; j < viewRect.Size.y; j++)
			{
				auto coord = viewRect.BottomLeft() + glm::ivec2(i, j);
				auto startPos = glm::vec2(coord) * (float)GameWorld::TILE_SIZE;


				int id = getId(coord);
				float d = distArray[id];
				if (d >= 16) continue;
				d = std::sqrt(1.f / (1.f + d * d)) * 3.f;
				renderer->Draw(startPos, glm::vec2(GameWorld::TILE_SIZE), glm::vec2(0),
					0.f, glm::vec4(d, d, d, 1));
			}
		}
	}
	renderer->End();
}

