#include "TRGame.h"

#include <Utils/Logging/Logger.h>
#include <Configs/ClientConfig.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/OpenGLTRGameGraphicsAPI.h>
#include <Core/Interfaces/ITRWindow.h>
#include <Assets/AssetsManager.h>
#include <Graphics/Renderers/OpenGLSpriteRenderer.h>

#include <Game/Worlds/GameWorld.h>
#include <Utils/Structures/Rect.h>
#include <glm/gtx/transform.hpp>

TRGame& TRGame::GetInstance() 
{
    static TRGame game;
    return game;
}

TRGame::TRGame() 
{
    _screenPosition = glm::vec2(0);
}

void TRGame::update()
{
}

void TRGame::draw()
{
    auto projection = glm::ortho(0.f, (float)_clientConfig->GetClientWidth(),
        0.f, (float)_clientConfig->GetClientHeight());
    auto translation = glm::translate(glm::vec3(-_screenPosition, 0));

    _spriteRenderer->Begin(projection * translation);
    {
        // calculate draw rect
        glm::ivec2 botLeft((int)(_screenPosition.x / GameWorld::TILE_SIZE), (int)(_screenPosition.y / GameWorld::TILE_SIZE));
        botLeft.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, botLeft.x));
        botLeft.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, botLeft.y));

        glm::ivec2 topRight((int)((_screenPosition.x + _clientConfig->GetClientWidth() + GameWorld::TILE_SIZE) / GameWorld::TILE_SIZE),
            (int)((_screenPosition.y + _clientConfig->GetClientHeight() + GameWorld::TILE_SIZE) / GameWorld::TILE_SIZE));
        topRight.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, topRight.x));
        topRight.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, topRight.y));

        RectI viewRect(botLeft, topRight - botLeft);
        _gameWorld->RenderWorld(_spriteRenderer.get(), viewRect);

        _screenPosition.x += 2;
    }
    _spriteRenderer->End();
}

void TRGame::loadSupportiveSystem()
{
    _clientConfig = std::make_shared<ClientConfig>();
    logTRHeaderInfos();
}

void TRGame::loadGraphicsSystem()
{
    _logger->LogInfo("Loading graphics system");

    // 这里 hard code 了一下OpenGL的API，因为第一阶段项目只考虑OpenGL
    OpenGLTRGameGraphicsAPIGenerator graphicsAPI;
    graphicsAPI.Initialize(_clientConfig.get());

    _graphicsDevice = graphicsAPI.GetGraphicsDevice();
    _gameWindow = graphicsAPI.GetWindow();
    _graphicsAPIUtils = graphicsAPI.GetGraphicsAPIUtils();
}

void TRGame::loadAssets()
{
    _logger->LogInfo("Loading assets");
    _assetsManager = std::make_shared<AssetsManager>();
}

void TRGame::postLoadEngines()
{
    _spriteRenderer = _graphicsDevice->CreateSpriteRenderer();
}

void TRGame::loadGameContent()
{
    _gameWorld = std::make_unique<GameWorld>(1000, 1000);

}

void TRGame::logTRHeaderInfos()
{
    _logger->LogInfo("TR Game Started");
    auto version = _clientConfig->GetVersionString();
    _logger->LogInfo("Current Version: %s", version.c_str());
    _logger->LogInfo("Initialization Stage");
}


TRGame::~TRGame() {
}

void TRGame::Initialize(int argc, char** argv)
{
    _logger = std::make_shared<Logger>();

    try {
        loadSupportiveSystem();
        loadGraphicsSystem();
        loadAssets();

        postLoadEngines();
        loadGameContent();
    }
    catch (std::exception ex) {
        _logger->LogError("Error: %s", ex.what());
        throw;
    }

    _logger->LogInfo("Finished initialization");
}

void TRGame::Run()
{
    double minElapsedTime = 1.0 / _clientConfig->GetFPSCap();
    double prevTimestamp = _graphicsAPIUtils->GetTime();
    while (!_gameWindow->ShouldClose()) {
        _gameWindow->BeginFrame();
        update();
        draw();
        _gameWindow->SwapBuffers();
        _gameWindow->PollEvents();
        while (_graphicsAPIUtils->GetTime() - prevTimestamp < minElapsedTime) {
            _gameWindow->PollEvents();
        }
        prevTimestamp = _graphicsAPIUtils->GetTime();
    }
}
