﻿#include "Engine.h"
#include "Application.h"

#include <Graphics/OpenGLProvider.h>
#include <Platform/GameWindow/GLFWGameWindow.h>

#include <Core/Utils/Logging/Logger.h>
#include <Core/Assets/AssetsManager.h>
#include <Core/Structures/EngineSettings.h>
#include <Core/Render/SpriteRenderer.h>
#include <Core/Gamplay/InputController.h>


TRV2_NAMESPACE_BEGIN

Engine* Engine::_instance = nullptr;

void Engine::SetApplication(Application* application)
{
	assert(application != nullptr);

	_application = application;
	useApplication();
}

void Engine::Run()
{
    try
    {
        double minElapsedTime = 1.0 / GetEngineSetting()->GetFPSCap();
        double prevTimestamp = _gameTimer.GetTimeFromGameStartInSeconds();

        auto controller = GetInputController();
        auto gameWindow = GetGameWindow();

        while (!gameWindow->ShouldClose())
        {
            auto currentTime = _gameTimer.GetTimeFromGameStartInSeconds();
            auto elapsed = currentTime - prevTimestamp;
            prevTimestamp = currentTime;

            controller->UpdateInput();
            gameWindow->BeginFrame();
            {
                _application->Update(elapsed);
                _application->Draw(elapsed);
            }
            gameWindow->EndFrame();
            controller->ClearInput();

            gameWindow->PollEvents();

            double t = _gameTimer.GetTimeFromGameStartInSeconds() - prevTimestamp;
            //_logger->LogInfo("%lf, %d\n", t, (int)(1 / t));
            while (_gameTimer.GetTimeFromGameStartInSeconds() - prevTimestamp < minElapsedTime)
            {
                gameWindow->PollEvents();
            }
        }

        _application->Exit();
    }
    catch (std::exception ex)
    {
        _logger->LogError("Error: %s", ex.what());
        throw;
    }
}
Engine::Engine(int argc, char** argv, Application* application)
{
    _instance = this;
    _logger = std::make_unique<Logger>();
    try
    {
        loadLaunchSettings();
        loadSupportiveSystem();
        loadGraphicsSystem();
        loadResources();

        SetApplication(application);
    }
    catch (std::exception ex)
    {
        _logger->LogError("Error: %s", ex.what());
        throw;
    }
}

Engine::~Engine()
{}

void Engine::loadSupportiveSystem()
{
    _logger->LogInfo("Loading platform specifics");
    _gameWindow = std::make_shared<GLFWGameWindow>(*_engineSettings);
    _inputController = std::make_shared<InputController>();
}

void Engine::loadGraphicsSystem()
{
    _logger->LogInfo("Loading graphics system");
    _graphicsProvider = std::make_shared<OpenGLProvider>(*_engineSettings);
}

void Engine::loadLaunchSettings()
{
    _logger->LogInfo("TR Engine Started");
    _engineSettings = std::make_shared<EngineSettings>();
    _logger->LogInfo("Current Engine Version: %s", _engineSettings->GetVersionString());
}

void Engine::loadResources()
{
    _assetsManager = std::make_shared<AssetsManager>(_graphicsProvider->GetGraphicsResourceManager());

    _spriteRenderer = std::make_shared<SpriteRenderer>(GetGraphicsDevice(), _assetsManager->GetShader("builtin::sprite"),
        _assetsManager->GetTexture2D("builtin::sprite"));
}

void Engine::useApplication()
{
    assert(_application != nullptr);
    _application->Initialize(this);
}
TRV2_NAMESPACE_END