#include "TRGame.h"
#include <Utils/Logging/Logger.h>
#include <Configs/ClientConfig.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/OpenGLTRGameGraphicsAPI.h>
#include <Core/Interfaces/ITRWindow.h>
#include <Assets/AssetsManager.h>
#include <Graphics/Renderers/OpenGLSpriteRenderer.h>

TRGame& TRGame::GetInstance() 
{
    static TRGame game;
    return game;
}

TRGame::TRGame() 
{
}

void TRGame::update()
{
}

void TRGame::draw()
{
    auto projection = glm::ortho(0.f, (float)_clientConfig->GetClientWidth(),
        0.f, (float)_clientConfig->GetClientHeight());
    _spriteRenderer->Begin(projection);
    {
        for (int i = 0; i <= 400; i++) {
            float x = i / 400.f;
            for (int j = 0; j <= 300; j++) {
                float y = j / 300.f;
                _spriteRenderer->Draw(glm::vec2(i * 2, j * 2), glm::vec2(2, 2),
                    glm::vec2(0), glm::vec4(x, y, 1, 1));
            }
        }
        //_spriteRenderer->Draw(glm::vec2(200, 200), glm::vec2(200, 200),
        //    glm::vec2(0), glm::vec4(1, 0, 0, 1));
        //_spriteRenderer->Draw(glm::vec2(700, 300), glm::vec2(200, 200),
        //    glm::vec2(0), glm::vec4(0, 1, 0, 1));
    }
    _spriteRenderer->End();
}

void TRGame::loadSupportiveSystem()
{
    _clientConfig = std::make_shared<ClientConfig>();
    logTRHeaderInfos();
}

void TRGame::loadGraphicsSystem()
{
    _logger->LogInfo("Loading graphics system");

    // 这里 hard code 了一下OpenGL的API，因为第一阶段项目只考虑OpenGL
    OpenGLTRGameGraphicsAPIGenerator graphicsAPI;
    graphicsAPI.Initialize(_clientConfig.get());

    _graphicsDevice = graphicsAPI.GetGraphicsDevice();
    _gameWindow = graphicsAPI.GetWindow();
    _graphicsAPIUtils = graphicsAPI.GetGraphicsAPIUtils();
}

void TRGame::loadAssets()
{
    _logger->LogInfo("Loading assets");
    _assetsManager = std::make_shared<AssetsManager>();
}

void TRGame::postSetUpContents()
{
    _spriteRenderer = _graphicsDevice->CreateSpriteRenderer();
}

void TRGame::logTRHeaderInfos()
{
    _logger->LogInfo("TR Game Started");
    auto version = _clientConfig->GetVersionString();
    _logger->LogInfo("Current Version: %s", version.c_str());
    _logger->LogInfo("Initialization Stage");
}


TRGame::~TRGame() {
}

void TRGame::Initialize(int argc, char** argv)
{
    _logger = std::make_shared<Logger>();

    try {
        loadSupportiveSystem();
        loadGraphicsSystem();
        loadAssets();

        postSetUpContents();
    }
    catch (std::exception ex) {
        _logger->LogError("Error: %s", ex.what());
        throw;
    }

    _logger->LogInfo("Finished initialization");
}

void TRGame::Run()
{
    double minElapsedTime = 1.0 / _clientConfig->GetFPSCap();
    double prevTimestamp = _graphicsAPIUtils->GetTime();
    while (!_gameWindow->ShouldClose()) {
        _gameWindow->BeginFrame();
        update();
        draw();
        _gameWindow->SwapBuffers();
        _gameWindow->PollEvents();
        printf("%lf\n", _graphicsAPIUtils->GetTime() - prevTimestamp);
        while (_graphicsAPIUtils->GetTime() - prevTimestamp < minElapsedTime) {
            _gameWindow->PollEvents();
        }
        prevTimestamp = _graphicsAPIUtils->GetTime();
    }
}
