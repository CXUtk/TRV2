#include "TREngine.h"

#include <Utils/Logging/Logger.h>
#include <Configs/ClientConfig.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/OpenGLTRGameGraphicsAPI.h>
#include <Core/Interfaces/ITRWindow.h>
#include <Assets/AssetsManager.h>
#include <Utils/Structures/Rect.h>

TREngine::TREngine()
{
}

TREngine::~TREngine()
{
}

void TREngine::Initialize(int argc, char** argv)
{
    _logger = std::make_unique<Logger>();
    _clientConfig = std::make_shared<ClientConfig>();
    try {
        loadSupportiveSystem();
        loadGraphicsSystem();
    }
    catch (std::exception ex) {
        _logger->LogError("Error: %s", ex.what());
        throw;
    }
}

void TREngine::LoadAllAssets()
{
}

double TREngine::GetGameTime()
{
    return _graphicsAPIUtils->GetTime();
}

void TREngine::loadSupportiveSystem()
{
    _clientConfig = std::make_shared<ClientConfig>();
    logEngineInfo();
}

void TREngine::loadGraphicsSystem()
{
    _logger->LogInfo("Loading graphics system");

    // 这里 hard code 了一下OpenGL的API，因为第一阶段项目只考虑OpenGL
    OpenGLTRGameGraphicsAPIGenerator graphicsAPI(this);
    graphicsAPI.Initialize(_clientConfig.get());

    _graphicsDevice = graphicsAPI.GetGraphicsDevice();
    _gameWindow = graphicsAPI.GetWindow();
    _graphicsAPIUtils = graphicsAPI.GetGraphicsAPIUtils();

    _assetsManager = std::make_shared<AssetsManager>();
    _spriteRenderer = _graphicsDevice->CreateSpriteRenderer();
}


void TREngine::logEngineInfo()
{
    _logger->LogInfo("TR Engine Started");
    auto version = _clientConfig->GetVersionString();
    _logger->LogInfo("Current Engine Version: %s", version.c_str());
}
