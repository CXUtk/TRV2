#pragma once
#include <TRV2.h>
#include <memory>
#include <glm/glm.hpp>

class TRGame {
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

    ReadonlySharedPtrProperty(ClientConfig, ClientConfig, clientConfig);
    ReadonlySharedPtrProperty(Logger, Logger, logger);
    ReadonlySharedPtrProperty(ITRGraphicsDevice, GraphicsDevice, graphicsDevice);
    ReadonlySharedPtrProperty(ITRWindow, Window, gameWindow);
    ReadonlySharedPtrProperty(ITRGameGraphicsAPIUtils, GraphicsAPIUtils, graphicsAPIUtils);
    ReadonlySharedPtrProperty(OpenGLSpriteRenderer, SpriteRenderer, spriteRenderer);
    ReadonlySharedPtrProperty(AssetsManager, AssetsManager, assetsManager);
private:
    TRGame();

    // 核心流程
    void update();
    void draw();

    // 加载函数
    void loadSupportiveSystem();
    void loadGraphicsSystem();
    void loadAssets();

    void postLoadEngines();
    void loadGameContent();

    // Utils
    void logTRHeaderInfos();


    std::unique_ptr<GameWorld> _gameWorld;
    glm::vec2 _screenPosition;
};