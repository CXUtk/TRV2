#pragma once

class VideoSettings
{
public:
	VideoSettings();
	~VideoSettings();

	float GetPrevFrameBlendingRatio() const { return _prevFrameBlendingRatio; }
	bool EnabledSmoothLight() const { return _enableSmoothLight; }
	bool EnabledFrameBlending() const { return _enableFrameBlending; }
	const int GetLightingBlurCount() const { return _lightingBlurCount; }
	const float GetCameraChaseRatio() const { return _cameraChaseRatio; }

private:
	bool _enableSmoothLight = true;

	bool _enableFrameBlending = true;
	int _lightingBlurCount = 2;

	float _prevFrameBlendingRatio = 0.5f;
	float _cameraChaseRatio = 0.4f;
};