#include "TRGame.h"
#include <Utils/Logging/Logger.h>
#include <Configs/ClientConfig.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/OpenGLTRGameGraphicsAPI.h>
#include <Core/Interfaces/ITRWindow.h>
#include <Assets/AssetsManager.h>
#include <Graphics/Renderers/OpenGLSpriteRenderer.h>

TRGame& TRGame::GetInstance() 
{
    static TRGame game;
    return game;
}

TRGame::TRGame() 
{
}

void TRGame::update()
{
}

void TRGame::draw()
{
    _spriteRenderer->Draw();
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

void TRGame::postSetUpContents()
{
    _spriteRenderer = _graphicsDevice->CreateSpriteRenderer();
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

        postSetUpContents();
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
        while (_graphicsAPIUtils->GetTime() - prevTimestamp < minElapsedTime) {
            _gameWindow->PollEvents();
        }
        prevTimestamp = _graphicsAPIUtils->GetTime();
    }
}
