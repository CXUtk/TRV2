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
#include <TRGame/Worlds/GameWorld.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Lighting/Lighting.h>


#include <glm/gtx/transform.hpp>



TRGame* TRGame::_instance = nullptr;

TRGame::~TRGame()
{
}


TRGame::TRGame() : _projection(glm::identity<glm::mat4>()), _screenRect(),  
_engine(nullptr), _spriteRenderer(nullptr), _tileRect()
{
    _instance = this;
    _expScale = 0.f;
    _mouseDragStart = glm::vec2(0);
    _oldScreenPos = glm::vec2(0);
}



void TRGame::logGameInfo()
{
    _logger->Log(trv2::SeverityLevel::Info, "TR Game Started");
}

void TRGame::loadGameContent()
{
    _gameWorld = std::make_unique<GameWorld>(1000, 1000);

    auto controller = _engine->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();

    _projection = glm::ortho(0.f, (float)clientSize.x,
        0.f, (float)clientSize.y);

    _screenRect = trv2::Rect2D<float>(glm::vec2(0), clientSize);
    _mainPlayer = std::make_unique<Player>();

    trv2::TextureParameters texPara{};
    texPara.SampleMethod = trv2::TextureSampleMethod::NEAREST;
    texPara.TextureWarpMethod = trv2::TextureWarpMethod::CLAMP_TO_EDGE;

    _tileTarget = std::make_shared<trv2::RenderTarget2D>(_engine->GetGraphicsResourceManager(), clientSize, texPara);
    _shadowMap = std::make_shared< trv2::RenderTarget2D>(_engine->GetGraphicsResourceManager(), clientSize, texPara);

    _tileRect = _gameWorld->GetTileRect(_screenRect);

    texPara.SampleMethod = trv2::TextureSampleMethod::BI_LINEAR;
    _shadowMapSwap[0] = std::make_shared<trv2::RenderTarget2D>(_engine->GetGraphicsResourceManager(), _tileRect.Size, texPara);
    _shadowMapSwap[1] = std::make_shared<trv2::RenderTarget2D>(_engine->GetGraphicsResourceManager(), _tileRect.Size, texPara);
}


void TRGame::Initialize(trv2::Engine* engine)
{
    _logger = std::make_unique<trv2::Logger>();

    _engine = engine;
    _spriteRenderer = _engine->GetSpriteRenderer();

    logGameInfo();
    loadGameContent();
}



void TRGame::Update(double deltaTime)
{
    auto controller = _engine->GetInputController();
    auto window = _engine->GetGameWindow();
    auto clientSize = window->GetWindowSize();


    auto pos = controller->GetMousePos();

    // 缩放视距
    float factor = std::exp(_expScale);
    //if (controller->GetScrollValue().y != 0)
    //{
    //    _expScale += controller->GetScrollValue().y * 0.1f;
    //    factor = std::exp(_expScale);

    //    auto unproject = glm::inverse(_projection);
    //    
    //    auto mouseScreen = controller->GetMousePos();
    //    auto mousePos = controller->GetMousePos() / glm::vec2(clientSize) * 2.f - glm::vec2(1.f);
    //    auto worldPos = glm::vec2(unproject * glm::vec4(mousePos, 0, 1));

    //    _screenRect.Position = glm::vec2(worldPos.x - mouseScreen.x / factor, worldPos.y - mouseScreen.y / factor);
    //    _screenRect.Size = glm::vec2(clientSize) / factor;
    //}



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

    _mainPlayer->Update();

    _screenRect.Size = window->GetWindowSize();
    _screenRect.Position = glm::mix(_screenRect.Position, _mainPlayer->GetPlayerHitbox().Center() - _screenRect.Size * 0.5f, 0.4f);
    _tileRect = _gameWorld->GetTileRect(_screenRect);
}

void TRGame::Draw(double deltaTime)
{
    auto graphicsDevice = _engine->GetGraphicsDevice();
    auto assetManager = _engine->GetAssetsManager();
    trv2::BatchSettings defaultSetting{};

    glm::mat4 renderWorldProjection = glm::ortho(_screenRect.Position.x, _screenRect.Position.x + _screenRect.Size.x,
        _screenRect.Position.y, _screenRect.Position.y + _screenRect.Size.y);
    glm::mat4 renderScreenProjection = glm::ortho(0.f, _screenRect.Size.x,
        0.f, _screenRect.Size.y);
    glm::mat4 renderTileProjection = glm::ortho(0.f, (float)_tileRect.Size.x,
        0.f, (float)_tileRect.Size.y);


    _shadowMapSwap[0]->Resize(_tileRect.Size);
    _shadowMapSwap[1]->Resize(_tileRect.Size);

    // render shadow map
    graphicsDevice->SwitchRenderTarget(trv2::ptr(_shadowMapSwap[0]));
    graphicsDevice->Clear(glm::vec4(0));
    Lighting::CalculateLight(_spriteRenderer, renderTileProjection, trv2::ptr(_gameWorld), _screenRect);


    // Blur shadow map
    for (int i = 0; i < 2; i++)
    {
        int index = i & 1;
        graphicsDevice->SwitchRenderTarget(trv2::ptr(_shadowMapSwap[!index]));
        graphicsDevice->Clear(glm::vec4(0));
        //defaultSetting.Shader = nullptr;
        defaultSetting.Shader = assetManager->GetShader("blur");
        _spriteRenderer->Begin(renderTileProjection, defaultSetting);
        {
            graphicsDevice->UseShader(defaultSetting.Shader);
            graphicsDevice->BindTexture2DSlot(1, _shadowMapSwap[index]->GetTexture2D());
            defaultSetting.Shader->SetParameter1i("uOriginalMap", 1);
            defaultSetting.Shader->SetParameter1i("horizontal", (int)(!index));
            
            _spriteRenderer->Draw(glm::vec2(0), _tileRect.Size,
                glm::vec2(0), 0.f, glm::vec4(1));
        }
        _spriteRenderer->End();
    }


    // render tile map
    graphicsDevice->SwitchRenderTarget(trv2::ptr(_tileTarget));
    graphicsDevice->Clear(glm::vec4(0));
    //renderWorldProjection = glm::ortho(_screenRect.Position.x, _screenRect.Position.x + (float)_tileTarget->GetWidth(),
    //    _screenRect.Position.y, _screenRect.Position.y + (float)_tileTarget->GetHeight());
    _gameWorld->RenderWorld(renderWorldProjection, _spriteRenderer, _screenRect);


    graphicsDevice->SwitchRenderTarget(trv2::ptr(_shadowMap));
    graphicsDevice->Clear(glm::vec4(0));
    defaultSetting.Shader = nullptr;
    _spriteRenderer->Begin(renderWorldProjection, defaultSetting);
    {
        auto texture = _shadowMapSwap[0]->GetTexture2D();
        _spriteRenderer->Draw(texture, _tileRect.Position * 16, _tileRect.Size * 16,
            glm::vec2(0), 0.f, glm::vec4(1));
    }
    _spriteRenderer->End();



    // Blending
    
    defaultSetting.Shader = assetManager->GetShader("blendShadow");
    graphicsDevice->UseShader(defaultSetting.Shader);
    _projection = glm::ortho(0.f, _screenRect.Size.x,
         0.f, _screenRect.Size.y);
    graphicsDevice->SwitchRenderTarget(nullptr);
    graphicsDevice->Clear(glm::vec4(0));
    _spriteRenderer->Begin(_projection, defaultSetting);
    {
        graphicsDevice->BindTexture2DSlot(1, _tileTarget->GetTexture2D());
        graphicsDevice->BindTexture2DSlot(2, _shadowMap->GetTexture2D());
        defaultSetting.Shader->SetParameter1i("uOriginalMap", 1);
        defaultSetting.Shader->SetParameter1i("uShadowMap", 2);

        _spriteRenderer->Draw(glm::vec2(0), _screenRect.Size, glm::vec2(0), 0.f, glm::vec4(1));
    }
    _spriteRenderer->End();

    _mainPlayer->Draw(renderWorldProjection, _spriteRenderer);
}

void TRGame::Exit()
{
    
}
