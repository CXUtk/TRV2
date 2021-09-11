#include "TRGame.hpp"

#include <TREngine/Engine.h>
#include <TREngine/Core/Render/SpriteRenderer.h>
#include <TREngine/Core/Utils/Utils.h>

#include <TREngine/Core/Gamplay/InputController.h>
#include <TREngine/Core/Utils/Logging/Logger.h>

#include <TREngine/Platform/Platform_Interfaces.h>
#include <TRGame/Worlds/GameWorld.hpp>

#include <glm/gtx/transform.hpp>


TRGame* TRGame::_instance = nullptr;

TRGame::~TRGame()
{
}


TRGame::TRGame() : _projection(glm::identity<glm::mat4>()), _screenRect(),  _engine(nullptr), _spriteRenderer(nullptr)
{
    _instance = this;
    _expScale = 0.f;
    _mouseDragStart = glm::vec2(0);
    _oldScreenPos = glm::vec2(0);
}



void TRGame::logGameInfo()
{
    _logger->LogInfo("TR Game Started");
}

void TRGame::loadGameContent()
{
    _gameWorld = std::make_unique<GameWorld>(1000, 1000);
}


void TRGame::Initialize(trv2::Engine* engine)
{
    _logger = std::make_unique<trv2::Logger>();

    _engine = engine;
    _spriteRenderer = _engine->GetSpriteRenderer();

    logGameInfo();
    loadGameContent();

    auto controller = _engine->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();

    _projection = glm::ortho(0.f, (float)clientSize.x,
        0.f, (float)clientSize.y);

    _screenRect = trv2::Rect2D<float>(glm::vec2(0), clientSize);
}



void TRGame::Update(double deltaTime)
{
    auto controller = _engine->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();

    auto moveVal = 0.05f * _screenRect.Size.x;
    if (controller->IsKeyDowned(trv2::KeyCode::TRV2_A_KEY))
    {
        _screenRect.Position.x -= moveVal;
    }
    else if (controller->IsKeyDowned(trv2::KeyCode::TRV2_D_KEY))
    {
        _screenRect.Position.x += moveVal;
    }
    else if (controller->IsKeyDowned(trv2::KeyCode::TRV2_W_KEY))
    {
        _screenRect.Position.y += moveVal;
    }
    else if (controller->IsKeyDowned(trv2::KeyCode::TRV2_S_KEY))
    {
        _screenRect.Position.y -= moveVal;
    }

    auto pos = controller->GetMousePos();

    float factor = std::exp(_expScale);
    if (controller->GetScrollValue().y != 0)
    {
        _expScale += controller->GetScrollValue().y * 0.1f;
        factor = std::exp(_expScale);

        auto unproject = glm::inverse(_projection);
        
        auto mouseScreen = controller->GetMousePos();
        auto mousePos = controller->GetMousePos() / glm::vec2(clientSize) * 2.f - glm::vec2(1.f);
        auto worldPos = glm::vec2(unproject * glm::vec4(mousePos, 0, 1));

        _screenRect.Position = glm::vec2(worldPos.x - mouseScreen.x / factor, worldPos.y - mouseScreen.y / factor);
        _screenRect.Size = glm::vec2(clientSize) / factor;
    }


    if (controller->IsMouseJustPressed(trv2::MouseButtonCode::LEFT_BUTTON))
    {
        _mouseDragStart = controller->GetMousePos();
        _oldScreenPos = _screenRect.Position;
    }
    if (controller->IsMouseDowned(trv2::MouseButtonCode::LEFT_BUTTON))
    {
        auto moveDir = (controller->GetMousePos() - _mouseDragStart) / factor;
        _screenRect.Position = _oldScreenPos - moveDir;
    }

    _projection = glm::ortho(_screenRect.Position.x, _screenRect.Position.x + _screenRect.Size.x,
    _screenRect.Position.y, _screenRect.Position.y + _screenRect.Size.y);
}

void TRGame::Draw(double deltaTime)
{
    auto window = _engine->GetGameWindow();
    auto graphicsDevice = _engine->GetGraphicsDevice();
    graphicsDevice->Clear(glm::vec4(0));

    _gameWorld->RenderWorld(_projection, _spriteRenderer, _screenRect);
}

void TRGame::Exit()
{}
