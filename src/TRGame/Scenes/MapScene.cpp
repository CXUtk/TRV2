#include "MapScene.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Worlds/GameWorld.h>

#include <TREngine/Engine.h>
#include <TREngine/Core/gameplay.h>
#include <TREngine/Core/render.h>
#include <TREngine/Platform/Platform_Interfaces.h>

MapScene::MapScene(trv2::Engine* engine, TRGame* game)
    :Scene(engine), _game(game)
{}

MapScene::~MapScene()
{}

void MapScene::Update(double deltaTime)
{
    auto controller = _engine->GetInputController();
    auto mousePos = controller->GetMousePos();
    auto clientSize = _engine->GetGameWindow()->GetWindowSize();

    // Ëõ·ÅÊÓ¾à
    float factor = std::exp(_expScale);
    if (controller->GetScrollValue().y != 0)
    {
        _expScale += controller->GetScrollValue().y * 0.1f;
        factor = std::exp(_expScale);

        auto unproject = glm::inverse(_worldProjection);
        
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
}

void MapScene::Draw(double deltaTime)
{
    //auto gameWorld = _game->GetGameWorld();
    //auto mapTex = gameWorld->GetMapTexture();


}

void MapScene::FocusOnPlayer()
{
    auto player = _game->GetLocalPlayer();
    auto center = player->GetPlayerHitbox().Center();
    auto clientSize = _engine->GetGameWindow()->GetWindowSize();

    _expScale = -1.f;
    float factor = std::exp(_expScale);
    
    _screenRect.Position = glm::vec2(center.x - clientSize.x * 0.5f / factor, center.x - clientSize.y * 0.5f / factor);
    _screenRect.Size = glm::vec2(clientSize) / factor;
}
