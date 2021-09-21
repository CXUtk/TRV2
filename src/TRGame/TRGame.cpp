#include "TRGame.hpp"

#include <TREngine/Engine.h>
#include <TREngine/Core/Render/SpriteRenderer.h>
#include <TREngine/Core/Utils/Utils.h>

#include <TREngine/Core/Gamplay/InputController.h>
#include <TREngine/Core/Utils/Logging/Logger.h>
#include <TREngine/Core/Render/RenderTarget2D.h>
#include <TREngine/Core/Assets/AssetsManager.h>
#include <TREngine/Core/Render/ShaderProgram.h>
#include <TREngine/Core/Render/Texture2D.h>

#include <TREngine/Platform/Platform_Interfaces.h>
#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Player/Player.h>
#include <TRGame/Lighting/Lighting.h>
#include <TRGame/Scenes/MainGameScene.h>
#include <TRGame/Scenes/MapScene.h>


#include <glm/gtx/transform.hpp>



TRGame* TRGame::_instance = nullptr;

TRGame::~TRGame()
{
}


TRGame::TRGame()
{
    _instance = this;
}



void TRGame::logGameInfo()
{
    _logger->Log(trv2::SeverityLevel::Info, "TR Game Started");
}

void TRGame::loadGameContent()
{
    _gameWorld = std::make_unique<GameWorld>(1000, 1000);
    _mainPlayer = std::make_unique<Player>();

    _mainGameScene = std::make_unique<MainGameScene>(_engine, this);
    _mapScene = std::make_unique<MapScene>(_engine, this);
}




void TRGame::Initialize(trv2::Engine* engine)
{
    _logger = std::make_unique<trv2::Logger>();

    _engine = engine;

    logGameInfo();
    loadGameContent();
}



void TRGame::Update(double deltaTime)
{
    if (_isMapEnabled)
    {
        _mapScene->Update(deltaTime);
    }
    else
    {
        _mainGameScene->Update(deltaTime);
    }
}

void TRGame::Draw(double deltaTime)
{
    if (_isMapEnabled)
    {
        _mapScene->Draw(deltaTime);
    }
    else
    {
        _mainGameScene->Draw(deltaTime);
    }
}

void TRGame::Exit()
{
    
}
