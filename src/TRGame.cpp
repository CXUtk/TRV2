#include "TRGame.h"
#include <Utils/Logging/Logger.h>
#include <Configs/ClientConfig.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/OpenGLTRGameGraphicsAPI.h>
#include <Core/Interfaces/ITRWindow.h>

TRGame& TRGame::GetInstance() 
{
    static TRGame game;
    return game;
}

TRGame::TRGame() 
{
}

void TRGame::loadSupportiveSystem()
{
    _logger = std::make_shared<Logger>();
    _clientConfig = std::make_shared<ClientConfig>();
    logTRHeaderInfos();
}

void TRGame::loadGraphicsSystem()
{
    _logger->LogInfo("Loading graphics system");

    OpenGLTRGameGraphicsAPI graphicsAPI;
    graphicsAPI.Initialize(_clientConfig.get());

    _graphicsDevice = graphicsAPI.GetGraphicsDevice();
    _gameWindow = graphicsAPI.GetWindow();
    _graphicsAPIUtils = graphicsAPI.GetGraphicsAPIUtils();
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
    loadSupportiveSystem();
    loadGraphicsSystem();

    _logger->LogInfo("Finished initialization");
}

void TRGame::Run()
{
    double minElapsedTime = 1.0 / _clientConfig->GetFPSCap();
    double prevTimestamp = _graphicsAPIUtils->GetTime();
    while (!_gameWindow->ShouldClose()) {
        _gameWindow->BeginFrame();

        _gameWindow->SwapBuffers();

        while (_graphicsAPIUtils->GetTime() - prevTimestamp < minElapsedTime) {
            _gameWindow->PollEvents();
        }
        prevTimestamp = _graphicsAPIUtils->GetTime();
    }
}
