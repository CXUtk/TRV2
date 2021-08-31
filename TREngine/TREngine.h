#pragma once
#include <TREngine_Interfaces.h>
#include <memory>
#include <glm/glm.hpp>

class TREngine
{
public:
    TREngine();
    ~TREngine();

    void Initialize(int argc, char** argv);
    void LoadAllAssets();

    double GetGameTime();

    ReadonlySharedPtrProperty(ClientConfig, ClientConfig, clientConfig);
    ReadonlySharedPtrProperty(ITRGraphicsDevice, GraphicsDevice, graphicsDevice);
    ReadonlySharedPtrProperty(ITRWindow, Window, gameWindow);
    ReadonlySharedPtrProperty(ITRGameGraphicsAPIUtils, GraphicsAPIUtils, graphicsAPIUtils);
    ReadonlySharedPtrProperty(OpenGLSpriteRenderer, SpriteRenderer, spriteRenderer);
    ReadonlySharedPtrProperty(AssetsManager, AssetsManager, assetsManager);
    ReadonlySharedPtrProperty(Logger, Logger, logger);
private:
    // 加载函数
    void loadSupportiveSystem();
    void loadGraphicsSystem();

    // Utils
    void logEngineInfo();
};