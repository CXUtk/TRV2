#include "Lighting.h"
#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/Tile.h>

#include <algorithm>
#include <vector>
#include <queue>

#include <TREngine/Core/Render/SpriteRenderer.h>

constexpr int VIS_SIZE = 128 * 128;

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

static constexpr int dX[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
static constexpr int dY[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
static constexpr float distA[8] = { 1.f, 1.f, 1.f, 1.f, 1.4142f, 1.4142f, 1.4142f, 1.4142f };
float distArray[VIS_SIZE];
bool visArray[VIS_SIZE];

constexpr float MAXDIST = 16.f;

void Lighting::CalculateLight(trv2::SpriteRenderer* renderer, const glm::mat4& projection, 
	const GameWorld* world, const trv2::Rect2D<float>& screenRect)
{
	auto tileRect = GameWorld::GetTileRect(screenRect);

	auto player = TRGame::GetInstance()->GetLocalPlayer();


	for (int i = 0; i < tileRect.Size.x * tileRect.Size.y; i++)
	{
		distArray[i] = MAXDIST;
		visArray[i] = false;
	}

	glm::ivec2 lightTile = GameWorld::GetUpperWorldCoord(player->GetPlayerHitbox().BottomLeft());

	auto getId = [&tileRect](glm::ivec2 pos) {
		int x = pos.x;
		int y = pos.y;
		return y * tileRect.Size.x + x;
	};

	auto valid = [&tileRect, world](glm::ivec2 pos, bool isSolid) {
		if (pos.x < tileRect.Position.x || pos.x >= tileRect.Position.x + tileRect.Size.x
			|| pos.y < tileRect.Position.y || pos.y >= tileRect.Position.y + tileRect.Size.y) return false;
		const auto& tile = world->GetTile(pos);
		if (tile.IsEmpty())return true;
		//if (isSolid) return false;
		return true;
	};

	std::priority_queue<LightNode> Q;

	auto v = lightTile - tileRect.Position;

	if (v.x >= 0 && v.x < tileRect.Size.x && v.y >= 0 && v.y < tileRect.Size.y)
	{
		int id = getId(v);
		distArray[id] = 0.f;
		Q.push({ lightTile, 0.f });
	}

	while (!Q.empty())
	{
		LightNode node = Q.top();
		Q.pop();

		int curId = getId(node.Pos - tileRect.Position);
		if (visArray[curId]) continue;
		visArray[curId] = true;

		bool solid = world->GetTile(node.Pos).IsSolid();
		for (int i = 0; i < 8; i++)
		{
			glm::ivec2 nxtPos(node.Pos.x + dX[i], node.Pos.y + dY[i]);
			if (valid(nxtPos, solid))
			{
				int nxtId = getId(nxtPos - tileRect.Position);
				if (distArray[nxtId] > distA[i] + distArray[curId])
				{
					distArray[nxtId] = distA[i] + distArray[curId];
					Q.push({ nxtPos, distArray[nxtId] });
				}
			}
		}
	}
	trv2::BatchSettings setting{};
	renderer->Begin(projection, setting);
	{
		auto start = glm::vec2(tileRect.Position);
		for (int i = 0; i < tileRect.Size.x; i++)
		{
			for (int j = 0; j < tileRect.Size.y; j++)
			{
				auto coord = tileRect.BottomLeft() + glm::ivec2(i, j);
				auto startPos = glm::vec2(coord) * (float)GameWorld::TILE_SIZE;
				auto& tile = world->GetTile(coord);
				if (tile.IsEmpty())
				{
					renderer->Draw(glm::ivec2(i, j), glm::vec2(1), glm::vec2(0),
						0.f, glm::vec4(1));
					continue;
				}
				int id = getId(glm::ivec2(i, j));
				float d = distArray[id];
				if (d >= MAXDIST) continue;

				d = glm::smoothstep(0.f, 1.f, 1.f - d / MAXDIST);
				renderer->Draw(glm::ivec2(i, j), glm::vec2(1), glm::vec2(0),
					0.f, glm::vec4(1) * d);
			}
		}
	}
	renderer->End();
}

