#include "TRGame.hpp"

#include <TREngine/TREngine.hpp>
#include <TREngine/Graphics/Renderers/SpriteRenderer.hpp>
#include <TREngine/Utils/Structures/Rect.hpp>

#include <Platform/Platform_Interfaces.hpp>

#include <Platform/GameTimer/GLFWGameTimer.hpp>
#include <Platform/GameWindow/GLFWGameWindow.hpp>
#include <Platform/InputController/GLFWInputController.hpp>

#include <TRGame/Worlds/GameWorld.hpp>

#include <glm/gtx/transform.hpp>


TRGame* TRGame::_instance = nullptr;

TRGame::~TRGame()
{
}


TRGame::TRGame() : _projection(glm::identity<glm::mat4>()), _screenRect(),  _engine(nullptr)
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


void TRGame::Initialize(trv2::IEngine* engine)
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

    _screenRect = trv2::Rect(glm::vec2(0), clientSize);
}



void TRGame::Update(double deltaTime)
{
    auto controller = _engine->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();

    auto moveVal = 0.05f * _screenRect.Size.x;
    if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_A_KEY))
    {
        _screenRect.Position.x -= moveVal;
    }
    else if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_D_KEY))
    {
        _screenRect.Position.x += moveVal;
    }
    else if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_W_KEY))
    {
        _screenRect.Position.y += moveVal;
    }
    else if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_S_KEY))
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


    if (controller->IsMouseClicked(trv2::TRV2MouseButtonCode::LEFT_BUTTON))
    {
        _mouseDragStart = controller->GetMousePos();
        _oldScreenPos = _screenRect.Position;
    }
    if (controller->IsMouseDowned(trv2::TRV2MouseButtonCode::LEFT_BUTTON))
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
    auto clientSize = window->GetWindowSize();
    auto controller = _engine->GetInputController();

    trv2::BatchSettings setting;
    setting.SpriteSortMode = trv2::SpriteSortMode::Deferred;
    _spriteRenderer->Begin(_projection, setting);
    {
        // calculate draw rect
        glm::ivec2 botLeft((int)(_screenRect.Position.x / GameWorld::TILE_SIZE), (int)(_screenRect.Position.y / GameWorld::TILE_SIZE));
        botLeft.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, botLeft.x));
        botLeft.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, botLeft.y));

        glm::ivec2 topRight((int)((_screenRect.Position.x + _screenRect.Size.x + GameWorld::TILE_SIZE - 1) / GameWorld::TILE_SIZE),
            (int)((_screenRect.Position.y + _screenRect.Size.y + GameWorld::TILE_SIZE - 1) / GameWorld::TILE_SIZE));
        topRight.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, topRight.x));
        topRight.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, topRight.y));

        trv2::RectI viewRect(botLeft, topRight - botLeft);
        _gameWorld->RenderWorld(_spriteRenderer, viewRect);

        //_spriteRenderer->Draw(glm::vec2((int)(worldPos.x / 16) * 16, (int)(worldPos.y / 16) * 16), glm::vec2(16), glm::vec2(0), 0.f, glm::vec4(1, 0, 0, 1));
    }
    _spriteRenderer->End();
}

void TRGame::Exit()
{}
