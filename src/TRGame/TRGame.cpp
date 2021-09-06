#include "TRGame.h"

#include <TREngine/TREngine.h>
#include <TREngine/Configs/EngineSettings.h>
#include <TREngine/Utils/Logging/Logger.h>
#include <TREngine/Core/Core_Interfaces.h>
#include <TREngine/Graphics/Renderers/SpriteRenderer.h>
#include <TREngine/Utils/Structures/Rect.h>

#include <TRGame/Worlds/GameWorld.h>


#include <glm/gtx/transform.hpp>


TRGame& TRGame::GetInstance()
{
    static TRGame game;
    return game;
}


TRGame::~TRGame()
{
}


TRGame::TRGame() : _screenPosition(glm::vec2(0)), _engine(nullptr)
{
}



void TRGame::logGameInfo()
{
    _logger->LogInfo("TR Game Started");
}

void TRGame::loadGameContent()
{
    _gameWorld = std::make_unique<GameWorld>(100, 100);
}

static float expV = 0;
static float factor = 1.0;
static glm::vec2 mouseDragStart;
static glm::vec2 oldScreenPos;
static glm::vec2 oldAnchorPos = glm::vec2(0);
static glm::mat4 P, V;

void TRGame::Initialize(trv2::TREngine* engine)
{
    _logger = std::make_unique<trv2::Logger>();

    _engine = engine;
    _spriteRenderer = _engine->CreateSpriteRenderer();

    logGameInfo();
    loadGameContent();

    auto controller = _engine->GetGameWindow()->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();

    P = glm::ortho(0.f, (float)clientSize.x,
        0.f, (float)clientSize.y);
    V = glm::identity<glm::mat4>();
}



void TRGame::Update(double deltaTime)
{
    auto controller = _engine->GetGameWindow()->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();

    if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_A_KEY))
    {
        _screenPosition.x -= 20;
    }
    else if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_D_KEY))
    {
        _screenPosition.x += 20;
    }
    else if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_W_KEY))
    {
        _screenPosition.y += 20;
    }
    else if (controller->IsKeyDowned(trv2::TRV2KeyCode::TRV2_S_KEY))
    {
        _screenPosition.y -= 20;
    }

    auto pos = controller->GetMousePos();

    if (controller->IsMouseClicked(trv2::TRV2MouseButtonCode::LEFT_BUTTON))
    {
        mouseDragStart = controller->GetMousePos();
        oldScreenPos = _screenPosition;
    }
    if (controller->IsMouseDowned(trv2::TRV2MouseButtonCode::LEFT_BUTTON))
    {
        auto moveDir = (controller->GetMousePos() - mouseDragStart) / factor;
        _screenPosition = oldScreenPos - moveDir;
    }

    if (controller->GetScrollValue().y != 0)
    {
        expV += controller->GetScrollValue().y * 0.1f;
        factor = std::exp(expV);
        auto unproject = glm::inverse(P * V);

        auto mousePos = controller->GetMousePos() / glm::vec2(clientSize) * 2.f - glm::vec2(1.f);
        auto worldPos = glm::vec2(unproject * glm::vec4(mousePos, 0, 1));
        oldAnchorPos = worldPos;
        printf("%lf %lf\n", worldPos.x, worldPos.y);

        V = glm::identity<glm::mat4>();
        V = glm::translate(V, glm::vec3(+worldPos, 0));
        V = glm::scale(V, glm::vec3(factor));
        V = glm::translate(V, glm::vec3(-worldPos, 0));
    }
}

void TRGame::Draw(double deltaTime)
{
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();
    auto controller = _engine->GetGameWindow()->GetInputController();

    auto unproject = glm::inverse(P * V);

    auto mousePos = controller->GetMousePos() / glm::vec2(clientSize) * 2.f - glm::vec2(1.f);
    auto worldPos = glm::vec2(unproject * glm::vec4(mousePos, 0, 1));
    //oldAnchorPos = worldPos;
    printf("%lf %lf\n", worldPos.x, worldPos.y);
    _spriteRenderer->Begin(P * V);
    {
        auto renderWidth = clientSize.x / factor;
        auto renderHeight = clientSize.y / factor;

        // calculate draw rect
        glm::ivec2 botLeft((int)(_screenPosition.x / GameWorld::TILE_SIZE), (int)(_screenPosition.y / GameWorld::TILE_SIZE));
        botLeft.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, botLeft.x));
        botLeft.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, botLeft.y));

        glm::ivec2 topRight((int)((_screenPosition.x + renderWidth + GameWorld::TILE_SIZE) / GameWorld::TILE_SIZE),
            (int)((_screenPosition.y + renderHeight + GameWorld::TILE_SIZE) / GameWorld::TILE_SIZE));
        topRight.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, topRight.x));
        topRight.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, topRight.y));

        //trv2::RectI viewRect(botLeft, topRight - botLeft);
        trv2::RectI viewRect(glm::vec2(0), glm::vec2(100, 100));
        _gameWorld->RenderWorld(trv2::ptr(_spriteRenderer), viewRect);

        _spriteRenderer->Draw(glm::vec2((int)(worldPos.x / 16) * 16, (int)(worldPos.y / 16) * 16), glm::vec2(16), glm::vec2(0), 0.f, glm::vec4(1, 0, 0, 1));
    }
    _spriteRenderer->End();
}

void TRGame::Exit()
{}
