#include "TRGame.h"

#include <TREngine/TREngine.h>
#include <TREngine/Configs/EngineSettings.h>
#include <TREngine/Utils/Logging/Logger.h>
#include <TREngine/Core/Interfaces/ITRWindow.h>
#include <TREngine/Graphics/Interfaces/ITRAPIUtils.h>
#include <TREngine/Graphics/Renderers/OpenGLSpriteRenderer.h>
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

void TRGame::Initialize(int argc, char** argv)
{
    _logger = std::make_unique<trv2::Logger>();
    logGameInfo();

    loadEngine(argc, argv);
    loadGameContent();
}

void TRGame::Run()
{
    auto& apiUtils = _engine->GetAPIUtils();
    auto& window = _engine->GetWindow();

    double minElapsedTime = 1.0 / _engine->GetEngineSetting().GetFPSCap();
    double prevTimestamp = apiUtils.GetTime();

    while (!window.ShouldClose()) {
        window.BeginFrame();
        update();
        draw();
        window.SwapBuffers();
        window.PollEvents();

        auto elapsedTime = apiUtils.GetTime() - prevTimestamp;
        _logger->LogDebug("Elapsed Time: %lf, FPS: %d", elapsedTime, (int)(1.0 / elapsedTime));

        while (apiUtils.GetTime() - prevTimestamp < minElapsedTime) {
            window.PollEvents();
        }
        prevTimestamp = apiUtils.GetTime();
    }
}

TRGame::TRGame() : _screenPosition(glm::vec2(0))
{
}

void TRGame::update()
{
}

void TRGame::draw()
{
    auto& config = _engine->GetEngineSetting();
    auto projection = glm::ortho(0.f, (float)config.GetClientWidth(),
    0.f, (float)config.GetClientHeight());
    auto translation = glm::translate(glm::vec3(-_screenPosition, 0));

    auto& spriteRenderer = _engine->GetSpriteRenderer();

    spriteRenderer.Begin(projection * translation);
    {
        // calculate draw rect
        glm::ivec2 botLeft((int)(_screenPosition.x / GameWorld::TILE_SIZE), (int)(_screenPosition.y / GameWorld::TILE_SIZE));
        botLeft.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, botLeft.x));
        botLeft.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, botLeft.y));

        glm::ivec2 topRight((int)((_screenPosition.x + config.GetClientWidth() + GameWorld::TILE_SIZE) / GameWorld::TILE_SIZE),
            (int)((_screenPosition.y + config.GetClientHeight() + GameWorld::TILE_SIZE) / GameWorld::TILE_SIZE));
        topRight.x = std::max(0, std::min(_gameWorld->GetWidth() - 1, topRight.x));
        topRight.y = std::max(0, std::min(_gameWorld->GetHeight() - 1, topRight.y));

        trv2::RectI viewRect(botLeft, topRight - botLeft);
        _gameWorld->RenderWorld(spriteRenderer, viewRect);

        _screenPosition.x += 2;
    }
    spriteRenderer.End();
}


void TRGame::logGameInfo()
{
    _logger->LogInfo("TR Game Started");
}

void TRGame::loadEngine(int argc, char** argv)
{
    _engine = std::make_unique<trv2::TREngine>();
    _engine->Initialize(argc, argv);
}

void TRGame::loadGameContent()
{
    _gameWorld = std::make_unique<GameWorld>(1000, 1000);
}