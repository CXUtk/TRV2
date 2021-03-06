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

	trv2::Rect2D<float> GetPlayerHitbox() const { return _playerHitBox; }

	void Update();
	void Draw(const glm::mat4& projection, trv2::SpriteRenderer* renderer);

	void SetVelocity(glm::vec2 value) { _velocity = value; }


	glm::vec2 GetVelocity() const { return _velocity; }
	int GetDirection() const { return _direction; }

private:
	trv2::Rect2D<float> _playerHitBox{};
	glm::vec2 _velocity{};

	bool _controlLeft = false;
	bool _controlRight = false;
	bool _controlJump = false;
	bool _controlUp = false;
	bool _controlDown = false;

	float _maxMovementSpeedX = 10.f;
	float _runAcceleration = 0.5f;
	float _gravity = 0.5f;
	float _jumpAcceleration = 15.f;

	bool _collideX = false;
	bool _collideY = false;

	int _direction = 1;

	void clearState();
	void updateInput();
	void handleMovement();
	void applyConstrains();

	trv2::Rectf tryMoveWithCollide(const trv2::Rectf& oldBox, glm::vec2 displacement, float timeDelta);
};