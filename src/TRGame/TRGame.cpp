#include "TRGame.hpp"

#include <TREngine/Engine.h>

#include <TREngine/Core/Utils/Utils.h>
#include <TREngine/Core/Utils/Logging/Logger.h>

#include <TREngine/Core/Gamplay/gameplay.h>
#include <TREngine/Core/Render/render.h>
#include <TREngine/Core/Structures/EngineSettings.h>

#include <TRGame/Player/Player.h>
#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Scenes/MainGameScene.h>
#include <TRGame/Scenes/MapScene.h>
#include <TRGame/Lighting/Lighting.h>
#include <TRGame/Worlds/WorldResources.h>


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
    _videoSettings = std::make_unique<VideoSettings>();
    _worldResources = std::make_unique<WorldResources>();

    _gameWorld = std::make_unique<GameWorld>();
    _lighting = std::make_unique<Lighting>();
    _mainPlayer = std::make_unique<Player>();

    _mainGameScene = std::make_unique<MainGameScene>(_engine, this);
    _mapScene = std::make_unique<MapScene>(_engine, this);

    _threadPool = std::make_unique<FixedThreadPool>(5);
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
    auto inputController = _engine->GetInputController();
    if (_gameState == GameState::MAIN)
    {
        _mainGameScene->Update(deltaTime);
    }
    else if (_gameState == GameState::MAP)
    {
        _mapScene->Update(deltaTime);
        if (inputController->IsKeyJustPressed(trv2::KeyCode::TRV2_ESC_KEY))
        {
            ChangeState(GameState::MAIN);
        }
    }

    if ((_gameState == GameState::MAIN ||
        _gameState == GameState::MAP) && inputController->IsKeyJustPressed(trv2::KeyCode::TRV2_M_KEY))
    {
        if (_gameState == GameState::MAIN)
        {
            ChangeState(GameState::MAP);
            _mapScene->FocusOnPlayer();
        }
        else if (_gameState == GameState::MAP)
        {
            ChangeState(GameState::MAIN);
        }
    }
}

void TRGame::Draw(double deltaTime)
{
    if (_gameState == GameState::MAIN)
    {
        _mainGameScene->Draw(deltaTime);
    }
    else if (_gameState == GameState::MAP)
    {
        _mapScene->Draw(deltaTime);
    }
}

void TRGame::Exit()
{
    
}

bool TRGame::ShouldSkipFrame(double elapsedFromFrameBegin)
{
    return false;//elapsedFromFrameBegin >= 1.f / _engine->GetEngineSetting()->GetFPSCap();
}

void TRGame::OnFrameEnd()
{
    if (_nextGameState != _gameState)
    {
        _gameState = _nextGameState;
    }
}

void TRGame::ChangeState(const GameState& state)
{
    _nextGameState = state;
}
