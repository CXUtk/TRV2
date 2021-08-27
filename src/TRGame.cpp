#include "TRGame.h"
#include <Graphics/GraphicsInterface/OpenGLGraphicsDevice.h>

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
    _graphicsDevice = std::make_shared<OpenGLGraphicsDevice>();

    _graphicsDevice->Initialize();
}

void TRGame::Run()
{
    _graphicsDevice->Loop();
}
