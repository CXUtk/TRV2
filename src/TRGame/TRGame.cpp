#include "TRGame.hpp"

#include <TREngine/Engine.h>
#include <TREngine/Core/Render/SpriteRenderer.h>
#include <TREngine/Core/Utils/Utils.h>

#include <TREngine/Core/Gamplay/InputController.h>
#include <TREngine/Core/Utils/Logging/Logger.h>
#include <TREngine/Core/Render/RenderTarget2D.h>
#include <TREngine/Core/Assets/AssetsManager.h>
#include <TREngine/Core/Render/ShaderProgram.h>

#include <TREngine/Platform/Platform_Interfaces.h>
#include <TRGame/Worlds/GameWorld.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Lighting/Lighting.h>


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
    _logger->Log(trv2::SeverityLevel::Info, "TR Game Started");
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
    _mainPlayer = std::make_unique<Player>();

    _tileTarget = std::make_shared<trv2::RenderTarget2D>(_engine->GetGraphicsResourceManager(), _screenRect.Size.x, _screenRect.Size.y);
    _shadowMap = std::make_shared<trv2::RenderTarget2D>(_engine->GetGraphicsResourceManager(), _screenRect.Size.x, _screenRect.Size.y);
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

    _screenRect.Size = window->GetWindowSize();
    _screenRect.Position = glm::mix(_screenRect.Position, _mainPlayer->GetPlayerHitbox().Center() - _screenRect.Size * 0.5f, 0.4f);


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
}

void TRGame::Draw(double deltaTime)
{
    auto graphicsDevice = _engine->GetGraphicsDevice();
    auto assetManager = _engine->GetAssetsManager();

    // render shadow map
    graphicsDevice->SwitchRenderTarget(trv2::ptr(_shadowMap));
    graphicsDevice->Clear(glm::vec4(0));
    glm::mat4 renderProjection = glm::ortho(_screenRect.Position.x, _screenRect.Position.x + (float)_shadowMap->GetWidth(),
        _screenRect.Position.y, _screenRect.Position.y + (float)_shadowMap->GetHeight());

    Lighting::CalculateLight(_spriteRenderer, renderProjection, trv2::ptr(_gameWorld), _screenRect);
    //_projection = glm::ortho(0.f, _screenRect.Size.x,
    //     0.f, _screenRect.Size.y);
    //_spriteRenderer->Begin(_projection, defaultSetting);
    //{
    //    _spriteRenderer->Draw(glm::vec2(0),
    //        glm::vec2(_shadowMap->GetWidth(), _shadowMap->GetHeight()), glm::vec2(0), 0.f, glm::vec4(1));
    //}
    //_spriteRenderer->End();

    // render tile map
    graphicsDevice->SwitchRenderTarget(trv2::ptr(_tileTarget));
    graphicsDevice->Clear(glm::vec4(0));
    renderProjection = glm::ortho(_screenRect.Position.x, _screenRect.Position.x + (float)_tileTarget->GetWidth(),
        _screenRect.Position.y, _screenRect.Position.y + (float)_tileTarget->GetHeight());
    _gameWorld->RenderWorld(renderProjection, _spriteRenderer, _screenRect);



    // Blending
    trv2::BatchSettings defaultSetting{};
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

    _mainPlayer->Draw(renderProjection, _spriteRenderer);
}

void TRGame::Exit()
{
    
}
