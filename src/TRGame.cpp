#include "TRGame.h"

#include <Utils/Logger.h>
#include <Config/ClientConfig.h>
#include <Graphics/GraphicsInterface/OpenGLGraphicsDevice.h>

TRGame& TRGame::GetInstance() 
{
    static TRGame game;
    return game;
}

TRGame::TRGame() {
}


TRGame::~TRGame() {
}

void TRGame::Initialize(int argc, char** argv)
{
    _logger = std::make_shared<Logger>();
    _logger->LogInfo("------------ TR Game -----------");
    _logger->LogInfo("Current Version %d.%d", 1, 0);
    _logger->LogInfo("Initialization Stage");

    _clientConfig = std::make_shared<ClientConfig>();
    _graphicsDevice = std::make_shared<OpenGLGraphicsDevice>(_clientConfig);
    
    _graphicsDevice->Initialize();
}

void TRGame::Run()
{
    _graphicsDevice->Loop();
}
