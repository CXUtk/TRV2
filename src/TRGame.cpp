#include "TRGame.h"
#include <Graphics/GraphicsInterface/OpenGLGraphicsDevice.h>
#include <Config/ClientConfig.h>

TRGame& TRGame::GetInstance() 
{
    static TRGame game;
    return game;
}

TRGame::~TRGame() {
}

void TRGame::Initialize(int argc, char** argv)
{
    _clientConfig = std::make_shared<ClientConfig>();
    _graphicsDevice = std::make_shared<OpenGLGraphicsDevice>(_clientConfig);
    
    _graphicsDevice->Initialize();
}

void TRGame::Run()
{
    _graphicsDevice->Loop();
}
