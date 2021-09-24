#include "MainGameScene.h"
#include <glm/gtx/transform.hpp>

#include <TREngine/Engine.h>
#include <TREngine/Platform/Platform_Interfaces.h>

#include <TREngine/Core/Gamplay/gameplay.h>
#include <TREngine/Core/Render/render.h>
#include <TREngine/Core/Assets/assets.h>

#include <TRGame/Lighting/Lighting.h>
#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Player/Player.h>
#include <TRGame/TRGame.hpp>


MainGameScene::MainGameScene(trv2::Engine* engine, TRGame* game)
	: trv2::Scene(engine), _game(game)
{
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();
    auto resourceManager = _engine->GetGraphicsResourceManager();
    auto gameWorld = _game->GetGameWorld();

    _screenRect = trv2::Rect2D<float>(glm::vec2(0), clientSize);

    trv2::TextureParameters texPara{};
    texPara.SampleMethod = trv2::TextureSampleMethod::NEAREST;
    texPara.WarpMethod = trv2::TextureWarpMethod::CLAMP_TO_EDGE;

    _tileTarget = std::make_shared<trv2::RenderTarget2D>(resourceManager, clientSize, texPara);
    _shadowMap = std::make_shared<trv2::RenderTarget2D>(resourceManager, clientSize, texPara);

    _tileRect = gameWorld->GetTileRect(_screenRect);

    texPara.SampleMethod = trv2::TextureSampleMethod::BI_LINEAR;
    _shadowMapSwap[0] = std::make_shared<trv2::RenderTarget2D>(resourceManager, _tileRect.Size, texPara);
    _shadowMapSwap[1] = std::make_shared<trv2::RenderTarget2D>(resourceManager, _tileRect.Size, texPara);
}

MainGameScene::~MainGameScene()
{}

void MainGameScene::Update(double deltaTime)
{
    auto controller = _engine->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();
    auto gameWorld = _game->GetGameWorld();
    auto localPlayer = _game->GetLocalPlayer();


    localPlayer->Update();

    _screenRect.Size = window->GetWindowSize();
    _screenRect.Position = glm::mix(_screenRect.Position, localPlayer->GetPlayerHitbox().Center() - _screenRect.Size * 0.5f, 0.4f);
    _tileRect = gameWorld->GetTileRect(_screenRect);


    // Set projection matricies
    _worldProjection = glm::ortho(_screenRect.Position.x, _screenRect.Position.x + _screenRect.Size.x,
        _screenRect.Position.y, _screenRect.Position.y + _screenRect.Size.y);
    _screenProjection = glm::ortho(0.f, _screenRect.Size.x,
         0.f, _screenRect.Size.y);
}

void MainGameScene::Draw(double deltaTime)
{
    drawShadowMaps();

    drawTiles();

    drawPlayers();
}

void MainGameScene::drawTiles()
{
    auto spriteRenderer = _engine->GetSpriteRenderer();
    auto graphicsDevice = _engine->GetGraphicsDevice();
    auto assetManager = _engine->GetAssetsManager();
    auto gameWorld = _game->GetGameWorld();

    // render tile map
    graphicsDevice->SwitchRenderTarget(trv2::ptr(_tileTarget));
    graphicsDevice->Clear(glm::vec4(0));
    gameWorld->RenderWorld(_worldProjection, spriteRenderer, _screenRect, trv2::ptr(_tileTarget));


    trv2::BatchSettings defaultSetting{};

    // Blending
    defaultSetting.Shader = assetManager->GetShader("blendShadow");
    graphicsDevice->UseShader(defaultSetting.Shader);
    graphicsDevice->SwitchRenderTarget(nullptr);
    graphicsDevice->Clear(glm::vec4(0));

    spriteRenderer->Begin(_screenProjection, defaultSetting);
    {
        graphicsDevice->BindTexture2DSlot(1, _tileTarget->GetTexture2D());
        graphicsDevice->BindTexture2DSlot(2, _shadowMap->GetTexture2D());
        defaultSetting.Shader->SetParameter1i("uOriginalMap", 1);
        defaultSetting.Shader->SetParameter1i("uShadowMap", 2);

        spriteRenderer->Draw(glm::vec2(0), _screenRect.Size, glm::vec2(0), 0.f, glm::vec4(1));
    }
    spriteRenderer->End();
}

void MainGameScene::drawShadowMaps()
{
    auto spriteRenderer = _engine->GetSpriteRenderer();
    auto graphicsDevice = _engine->GetGraphicsDevice();
    auto assetManager = _engine->GetAssetsManager();
    auto gameWorld = _game->GetGameWorld();

    glm::mat4 renderScreenProjection = glm::ortho(0.f, _screenRect.Size.x,
        0.f, _screenRect.Size.y);
    glm::mat4 renderTileProjection = glm::ortho(0.f, (float)_tileRect.Size.x,
        0.f, (float)_tileRect.Size.y);

    trv2::BatchSettings defaultSetting{};

    _shadowMapSwap[0]->Resize(_tileRect.Size);
    _shadowMapSwap[1]->Resize(_tileRect.Size);

    // render shadow map
    graphicsDevice->SwitchRenderTarget(trv2::ptr(_shadowMapSwap[0]));
    graphicsDevice->Clear(glm::vec4(0));
    Lighting::CalculateLight(spriteRenderer, renderTileProjection, gameWorld, _screenRect);


    // Blur shadow map
    for (int i = 0; i < 2; i++)
    {
        int index = i & 1;
        graphicsDevice->SwitchRenderTarget(trv2::ptr(_shadowMapSwap[!index]));
        graphicsDevice->Clear(glm::vec4(0));
        //defaultSetting.Shader = nullptr;
        defaultSetting.Shader = assetManager->GetShader("blur");
        spriteRenderer->Begin(renderTileProjection, defaultSetting);
        {
            graphicsDevice->UseShader(defaultSetting.Shader);
            graphicsDevice->BindTexture2DSlot(1, _shadowMapSwap[index]->GetTexture2D());
            defaultSetting.Shader->SetParameter1i("uOriginalMap", 1);
            defaultSetting.Shader->SetParameter1i("horizontal", (int)(!index));

            spriteRenderer->Draw(glm::vec2(0), _tileRect.Size,
                glm::vec2(0), 0.f, glm::vec4(1));
        }
        spriteRenderer->End();
    }

    graphicsDevice->SwitchRenderTarget(trv2::ptr(_shadowMap));
    graphicsDevice->Clear(glm::vec4(0));
    defaultSetting.Shader = nullptr;
    spriteRenderer->Begin(_worldProjection, defaultSetting);
    {
        auto texture = _shadowMapSwap[0]->GetTexture2D();
        spriteRenderer->Draw(texture, _tileRect.Position * 16, _tileRect.Size * 16,
            glm::vec2(0), 0.f, glm::vec4(1));
    }
    spriteRenderer->End();
}

void MainGameScene::drawPlayers()
{
    auto spriteRenderer = _engine->GetSpriteRenderer();
    auto localPlayer = _game->GetLocalPlayer();

    localPlayer->Draw(_worldProjection, spriteRenderer);
}
