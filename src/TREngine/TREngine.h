#pragma once
#include <TREngine_Interfaces.h>
#include <memory>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
class TREngine
{
public:
    TREngine();
    ~TREngine();

    void Initialize(int argc, char** argv);
    void LoadAllAssets();

    double GetGameTime();

    ReadonlyReferenceProperty(EngineSettings, EngineSetting, engineSetting);
    ReadonlyReferenceProperty(ITRGraphicsDevice, GraphicsDevice, graphicsDevice);
    ReadonlyReferenceProperty(ITRWindow, Window, gameWindow);
    ReadonlyReferenceProperty(ITRAPIUtils, APIUtils, apiUtils);
    ReadonlyReferenceProperty(ISpriteRenderer, SpriteRenderer, spriteRenderer);
    ReadonlyReferenceProperty(AssetsManager, AssetsManager, assetsManager);
    ReadonlyReferenceProperty(Logger, Logger, logger);
private:
    // 加载函数
    void loadSupportiveSystem();
    void loadGraphicsSystem();

    // Utils
    void logEngineInfo();
};
TRV2_NAMESPACE_END