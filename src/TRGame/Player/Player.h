#pragma once
#include <TRGame/TRGame_Interfaces.h>
#include <glm/glm.hpp>
#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>

class Player
{
public:
	Player();
	~Player();

	trv2::Rect2D<float> GetPlayerHitbox() const
	{
		return _playerHitBox;
	}

	void Update()
	{
		_playerHitBox.Position += _velocity;
	}

	void SetVelocity(glm::vec2 value)
	{
		_velocity = value;
	}

	glm::vec2 GetVelocity() const { return _velocity; }

private:
	trv2::Rect2D<float> _playerHitBox;
	glm::vec2 _velocity;
};