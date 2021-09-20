#pragma once
#include <Core.h>
#include <bitset>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN

class Scene
{
public:
	Scene(Engine* engine) : _engine(engine){}
	virtual ~Scene() = 0 {};

	virtual void Update(double deltaTime) = 0;
	virtual void Draw(double deltaTime) = 0;

protected:
	Engine* _engine;
};
TRV2_NAMESPACE_END