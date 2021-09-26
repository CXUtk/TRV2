#pragma once

class VideoSettings
{
public:
	VideoSettings();
	~VideoSettings();

	float GetPrevFrameBlendingRatio() const { return _prevFrameBlendingRatio; }
	bool EnabledSmoothLight() const { return _enableSmoothLight; }
	bool EnabledFrameBlending() const { return _enableFrameBlending; }

private:
	bool _enableSmoothLight = true;

	bool _enableFrameBlending = true;
	float _prevFrameBlendingRatio = 0.8f;
};