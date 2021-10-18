#pragma once
#include "LightCalculator.h"
#include "LightCommon.h"

#include <glm/glm.hpp>
#include <queue>
#include <algorithm>

struct LightNode
{
	glm::i16vec2 Pos;
	float Luminance;

	// Unused
	int k;

	bool operator<(const LightNode& B) const
	{
		return this->Luminance < B.Luminance;
	}
};

class BFSLightCalculator : public LightCalculator
{
public:
	BFSLightCalculator(LightCommon* lightCommon);
	virtual ~BFSLightCalculator() override;

	virtual void AddLight(const Light& light) override;
	virtual void ClearLights() override;
	virtual void Calculate() override;

private:

	LightCommon* _lightCommonData;

	float _luminances[LightCommon::CHANNELS][LightCommon::MAX_TILES];
	bool _visited[LightCommon::CHANNELS][LightCommon::MAX_TILES];
	std::priority_queue<LightNode> _lightQueue[LightCommon::CHANNELS];
	std::vector<Light> _lights;

	void calculateOneChannel(int channel);
	float calculateLuminance(glm::ivec2 worldCoord, int dir, float curLuminance);
};