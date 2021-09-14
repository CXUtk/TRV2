#include "Player.h"
#include <TRGame/TRGame.hpp>
#include <TREngine/Engine.h>
#include <TREngine/Core/Gamplay/InputController.h>
#include <TREngine/Core/Utils/Logging/Logger.h>
#include <TRGame/Worlds/GameWorld.hpp>
#include <TREngine/Core/Utils/Collision.h>
#include <TREngine/Core/Render/SpriteRenderer.h>

Player::Player()
{
	_playerHitBox.Size = glm::vec2(16, 32);
	_playerHitBox.Position = glm::vec2(16, 850);
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

	setting.BlendMode = trv2::BlendMode::AlphaBlend;
	
	renderer->Begin(projection, setting);
	{
		auto start = GameWorld::GetLowerWorldCoord(_playerHitBox.BottomLeft());
		trv2::RectI tileRect(start, 
			GameWorld::GetUpperWorldCoord(_playerHitBox.TopRight()) - start);
		for (int y = tileRect.Position.y; y <= tileRect.Position.y + tileRect.Size.y; y++)
		{
			for (int x = tileRect.Position.x; x <= tileRect.Position.x + tileRect.Size.x; x++)
			{
				auto fRect = trv2::Rectf(glm::vec2(x * GameWorld::TILE_SIZE, y * GameWorld::TILE_SIZE), glm::vec2(GameWorld::TILE_SIZE));
				renderer->Draw(fRect.Position, fRect.Size, glm::vec2(0), 0.f, glm::vec4(0.1, 0.6, 0.2, 0.5));
				if (world->GetTile(x, y).GetColor() == glm::vec3(1)) continue;
				if (trv2::RectIntersects(_playerHitBox, fRect))
				{
					renderer->Draw(fRect.Position, fRect.Size, glm::vec2(0), 0.f, glm::vec4(0.7, 0.1, 0.2, 0.5));
				}
			}
		}
	}
	renderer->End();
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

	_velocity.y = std::max(-16.f, _velocity.y - _gravity);
}

void Player::applyConstrains()
{
	auto world = TRGame::GetInstance()->GetGameWorld();

	float distance = glm::length(_velocity);
	auto unit = glm::normalize(_velocity);

	for (float step = 0; step < distance; step += 10)
	{
		auto oldHitbox = _playerHitBox;
		_playerHitBox.Position += unit * std::min(10.f, distance - step);

		auto start = GameWorld::GetLowerWorldCoord(_playerHitBox.BottomLeft());
		trv2::RectI tileRect(start,
			GameWorld::GetUpperWorldCoord(_playerHitBox.TopRight()) - start);

		for (int y = tileRect.Position.y; y <= tileRect.Position.y + tileRect.Size.y; y++)
		{
			for (int x = tileRect.Position.x; x <= tileRect.Position.x + tileRect.Size.x; x++)
			{
				if (world->GetTile(x, y).GetColor() == glm::vec3(1)) continue;
				auto fRect = trv2::Rectf(glm::vec2(x * GameWorld::TILE_SIZE, y * GameWorld::TILE_SIZE), glm::vec2(GameWorld::TILE_SIZE));
				if (trv2::RectIntersects(_playerHitBox, fRect))
				{
					auto side = trv2::GetCollisionSide(oldHitbox, _velocity, fRect);
					switch (side)
					{
					case trv2::CollisionSide::LEFT:
					{
						_playerHitBox.Position.x = fRect.Position.x - _playerHitBox.Size.x - 0.001;
						_velocity.x = 0;
						break;
					}
					case trv2::CollisionSide::RIGHT:
					{
						_playerHitBox.Position.x = fRect.Position.x + fRect.Size.x + 0.001;
						_velocity.x = 0;
						break;
					}
					case trv2::CollisionSide::TOP:
					{
						_playerHitBox.Position.y = fRect.Position.y + fRect.Size.y + 0.001;
						_velocity.y = 0;
						break;
					}
					case trv2::CollisionSide::BOTTOM:
					{
						_playerHitBox.Position.y = fRect.Position.y - _playerHitBox.Size.y - 0.001;
						_velocity.y = 0;
						break;
					}
					default:
						break;
					}
				}
			}
		}
	}
	

	_playerHitBox.Position.y = std::max(_playerHitBox.Position.y, 0.f);
}
