#include "TRGame.h"

#include <Utils/Logging/Logger.h>
#include <Configs/ClientConfig.h>
#include <Graphics/GraphicsInterface/OpenGLGraphicsDevice.h>

TRGame& TRGame::GetInstance() 
{
    static TRGame game;
    return game;
}

TRGame::TRGame() {
}

void TRGame::logTRHeaderInfos()
{
    _logger->LogInfo("TR Game Started");
    _logger->LogInfo("Current Version: %s", _clientConfig->GetVersionString().c_str());
    _logger->LogInfo("Initialization Stage");
}


TRGame::~TRGame() {
}

void TRGame::Initialize(int argc, char** argv)
{
    _logger = std::make_shared<Logger>();

    _clientConfig = std::make_shared<ClientConfig>();
    logTRHeaderInfos();

    _graphicsDevice = std::make_shared<OpenGLGraphicsDevice>(_clientConfig);
    
    _graphicsDevice->Initialize();
}

void TRGame::Run()
{
    _graphicsDevice->Loop();
}
