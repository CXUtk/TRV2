#include "Player.h"

#include <TRGame/TRGame.hpp>
#include <TREngine/Engine.h>
#include <TREngine/Core/Gamplay/InputController.h>
#include <TREngine/Core/Utils/Logging/Logger.h>
#include <TREngine/Core/Utils/Collision.h>
#include <TREngine/Core/Render/SpriteRenderer.h>

#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/Tile.hpp>

Player::Player()
{
	_playerHitBox.Size = glm::vec2(16, 32);
	_playerHitBox.Position = glm::vec2(1050, 14600);
}

Player::~Player()
{}

void Player::Update()
{
	clearState();
	updateInput();
	handleMovement();
	applyConstrains();
}

void Player::Draw(const glm::mat4& projection, trv2::SpriteRenderer* renderer)
{
	auto world = TRGame::GetInstance()->GetGameWorld();

	trv2::BatchSettings setting{};
	setting.SpriteSortMode = trv2::SpriteSortMode::Deferred;
	setting.Shader = nullptr;

	renderer->Begin(projection, setting);
	{
		renderer->Draw(_playerHitBox.Position, _playerHitBox.Size, glm::vec2(0), 0.f, glm::vec4(0.6, 0.9, 0.3, 1.0));
	}
	renderer->End();

	//setting.BlendMode = trv2::BlendingMode::AlphaBlend;
	//renderer->Begin(projection, setting);
	//{
	//	auto start = GameWorld::GetLowerWorldCoord(_playerHitBox.BottomLeft());
	//	trv2::RectI tileRect(start, 
	//		GameWorld::GetUpperWorldCoord(_playerHitBox.TopRight()) - start);
	//	for (int y = tileRect.Position.y; y <= tileRect.Position.y + tileRect.Size.y; y++)
	//	{
	//		for (int x = tileRect.Position.x; x <= tileRect.Position.x + tileRect.Size.x; x++)
	//		{
	//			auto fRect = trv2::Rectf(glm::vec2(x * GameWorld::TILE_SIZE, y * GameWorld::TILE_SIZE), glm::vec2(GameWorld::TILE_SIZE));
	//			renderer->Draw(fRect.Position, fRect.Size, glm::vec2(0), 0.f, glm::vec4(0.1, 0.6, 0.2, 0.5));
	//			if (world->GetTile(x, y).IsEmpty()) continue;
	//			if (trv2::RectIntersects(_playerHitBox, fRect))
	//			{
	//				renderer->Draw(fRect.Position, fRect.Size, glm::vec2(0), 0.f, glm::vec4(0.7, 0.1, 0.2, 0.5));
	//			}
	//		}
	//	}
	//}
	//renderer->End();
}

void Player::clearState()
{
	_controlLeft = false;
	_controlRight = false;
	_controlJump = false;
	_controlUp = false;
	_controlDown = false;
}

void Player::updateInput()
{
	auto inputControl = TRGame::GetInstance()->GetEngine()->GetInputController();
	if (inputControl->IsKeyDowned(trv2::KeyCode::TRV2_A_KEY))
	{
		_controlLeft = true;
	}
	if (inputControl->IsKeyDowned(trv2::KeyCode::TRV2_D_KEY))
	{
		_controlRight = true;
	}
	if (inputControl->IsKeyDowned(trv2::KeyCode::TRV2_W_KEY))
	{
		_controlUp = true;
	}
	if (inputControl->IsKeyDowned(trv2::KeyCode::TRV2_S_KEY))
	{
		_controlDown = true;
	}
	if (inputControl->IsKeyJustPressed(trv2::KeyCode::TRV2_SPACE_KEY))
	{
		_controlJump = true;
	}
}

void Player::handleMovement()
{
	if (_controlLeft)
	{
		_direction = -1;
		if (_velocity.x > -_maxMovementSpeedX)
		{
			_velocity.x -= _runAcceleration;
			if (_velocity.x < -_maxMovementSpeedX)
			{
				_velocity.x = -_maxMovementSpeedX;
			}
		}
	}
	if (_controlRight)
	{
		_direction = 1;
		if (_velocity.x < _maxMovementSpeedX)
		{
			_velocity.x += _runAcceleration;
			if (_velocity.x > _maxMovementSpeedX)
			{
				_velocity.x = _maxMovementSpeedX;
			}
		}
	}

	if (_controlJump)
	{
		if (_velocity.y < 0) _velocity.y = 0;
		_velocity.y += _jumpAcceleration;
	}

	if (!_controlLeft && !_controlRight)
	{
		if (_velocity.x > 0)
		{
			_velocity.x = std::max(0.f, _velocity.x - _runAcceleration * 2);
		}
		else
		{
			_velocity.x = std::min(0.f, _velocity.x + _runAcceleration * 2);
		}
	}

	_velocity.y = std::max(-30.f, _velocity.y - _gravity);
}


void Player::applyConstrains()
{
	float T = 0.f;
	while (T < 1.f)
	{
		auto len = glm::length(_velocity);
		float step = std::min(1.f - T, 16.f / len);
		_playerHitBox = tryMoveWithCollide(_playerHitBox, _velocity * step, step);
		T += step;
	}
	_playerHitBox.Position.y = std::max(_playerHitBox.Position.y, 0.f);
}

trv2::Rectf Player::tryMoveWithCollide(const trv2::Rectf& oldBox, glm::vec2 displacement, float timeDelta)
{ 
	auto world = TRGame::GetInstance()->GetGameWorld();

	trv2::Rectf newBox = oldBox;
	newBox.Position += displacement;

	auto start = GameWorld::GetLowerWorldCoord(newBox.BottomLeft());
	trv2::RectI tileRect(start,
		GameWorld::GetUpperWorldCoord(newBox.TopRight()) - start);

	std::vector<trv2::Interval> intervalV;
	std::vector<trv2::Interval> intervalH;
	std::vector<trv2::Interval> subjectInterval = trv2::GetCollidingSegments(oldBox, displacement);
	for (int y = tileRect.Position.y; y <= tileRect.Position.y + tileRect.Size.y; y++)
	{
		for (int x = tileRect.Position.x; x <= tileRect.Position.x + tileRect.Size.x; x++)
		{
			if (world->GetTile(x, y).IsEmpty()) continue;
			auto fRect = trv2::Rectf(glm::vec2(x * GameWorld::TILE_SIZE, y * GameWorld::TILE_SIZE), glm::vec2(GameWorld::TILE_SIZE));
			if (!trv2::RectIntersects(newBox, fRect)) continue;

			for (auto& inv : trv2::GetCollidingSegmentsRev(fRect, displacement))
			{
				if (inv.horizontal)
				{
					intervalH.push_back(inv);
				}
				else
				{
					intervalV.push_back(inv);
				}
			}
		}
	}

	float minTimeX = std::numeric_limits<float>::infinity(), minTimeY = std::numeric_limits<float>::infinity();
	for (auto& sub : subjectInterval)
	{
		float t = trv2::GetNearestCollisionTime(sub, displacement, sub.horizontal ? intervalH : intervalV);
		if (sub.horizontal)
		{
			minTimeY = std::min(minTimeY, t);
		}
		else
		{
			minTimeX = std::min(minTimeX, t);
		}
	}

	if (minTimeX != std::numeric_limits<float>::infinity() && minTimeX < minTimeY)
	{
		newBox.Position = oldBox.Position + displacement * minTimeX;

		_velocity.x = 0;
		displacement = _velocity * timeDelta * (1.f - minTimeX);
		if (minTimeY != std::numeric_limits<float>::infinity())
		{
			return tryMoveWithCollide(newBox, displacement, (1.f - minTimeX) * timeDelta);
		}
		else
		{
			newBox.Position += displacement;
		}
	}
	else if (minTimeY != std::numeric_limits<float>::infinity() && minTimeY <= minTimeX)
	{
		newBox.Position = oldBox.Position + displacement * minTimeY;

		_velocity.y = 0.f;
		displacement = _velocity * timeDelta * (1.f - minTimeY);
		if (minTimeX != std::numeric_limits<float>::infinity())
		{
			return tryMoveWithCollide(newBox, displacement, (1.f - minTimeY) * timeDelta);
		}
		else
		{
			newBox.Position += displacement;
		}
	}
	if (intervalH.size() > 0 &&
		intervalV.size() > 0 && minTimeY == std::numeric_limits<float>::infinity()
		&& minTimeX == std::numeric_limits<float>::infinity())
	{
		newBox = oldBox;
		if (std::abs(displacement.x) > std::abs(displacement.y))
		{
			newBox.Position.x += displacement.x;
		}
		else
		{
			newBox.Position.y += displacement.y;
		}
	}
	return newBox;
}
