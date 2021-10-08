#include "Engine.h"
#include "Application.h"

#include "Graphics/Graphics_opengl.h"
#include "Platform/Platform_glfw.h"

#include "Core/Utils/Logging/Logger.h"
#include "Core/Utils/GameTimer.h"
#include "Core/Assets/assets.h"
#include "Core/Render/render.h"
#include "Core/Gamplay/gameplay.h"
#include "Core/Structures/EngineSettings.h"

TRV2_NAMESPACE_BEGIN

Engine* Engine::_instance = nullptr;

void Engine::SetApplication(const std::shared_ptr<Application>& application)
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
        double prevTimestamp = _gameTimer->GetTimeFromGameStartInSeconds();

        auto controller = GetInputController();
        auto gameWindow = GetGameWindow();

        while (!gameWindow->ShouldClose())
        {
            auto currentTime = _gameTimer->GetTimeFromGameStartInSeconds();
            auto elapsed = currentTime - prevTimestamp;
            prevTimestamp = currentTime;

            controller->DispatchInput();
            gameWindow->BeginFrame();
            {
                _application->Update(elapsed);

                if (!_application->ShouldSkipFrame(_gameTimer->GetTimeFromGameStartInSeconds() - prevTimestamp))
                {
                    _application->Draw(elapsed);
                }
                _application->OnFrameEnd();
            }
            gameWindow->EndFrame();
            controller->ClearInput();

            gameWindow->PollEvents();

            double t = _gameTimer->GetTimeFromGameStartInSeconds() - prevTimestamp;
            // _logger->Log(trv2::SeverityLevel::Info, "%lf, %d", t, (int)(1 / t));
            while (_gameTimer->GetTimeFromGameStartInSeconds() - prevTimestamp < minElapsedTime)
            {
                gameWindow->PollEvents();
            }
        }

        _application->Exit();
    }
    catch (std::exception ex)
    {
        _logger->Log(SeverityLevel::Error, "Error: %s", ex.what());
        throw;
    }
}
Engine::Engine(int argc, char** argv, const std::shared_ptr<Application>& application)
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
        _logger->Log(SeverityLevel::Error, "Error: %s", ex.what());
        throw;
    }
}

Engine::~Engine()
{
    _application.reset();
    _spriteRenderer.reset();
    _assetsManager.reset();
    _graphicsProvider.reset();
}

void Engine::loadSupportiveSystem()
{
    _logger->Log(SeverityLevel::Info, "Loading platform specifics");
    _gameTimer = std::make_shared<GameTimer>();
    _platformProvider = std::make_shared<GLFWProvider>(trv2::cref(_engineSettings));
    _inputController = std::make_shared<InputController>();
}

void Engine::loadGraphicsSystem()
{
    _logger->Log(SeverityLevel::Info, "Loading graphics system");
    _graphicsProvider = std::make_shared<OpenGLProvider>(trv2::cref(_engineSettings));
}

void Engine::loadLaunchSettings()
{
    _logger->Log(SeverityLevel::Info, "TR Engine Started");
    _engineSettings = std::make_shared<EngineSettings>();
    _logger->Log(SeverityLevel::Info, "Current Engine Version: %s", _engineSettings->GetVersionString());
}

void Engine::loadResources()
{
    _assetsManager = std::make_shared<AssetsManager>(_graphicsProvider->GetGraphicsResourceManager());

    _spriteRenderer = std::make_shared<SpriteRenderer>(GetGraphicsDevice(), _assetsManager->GetShader("builtin::sprite"),
        _assetsManager->GetTexture2D("builtin::sprite"));

    _universalRenderer = std::make_shared<UniversalRenderer>();
}

void Engine::useApplication()
{
    assert(_application != nullptr);
    _application->Initialize(this);
}

IGraphicsDevice* Engine::GetGraphicsDevice()
{
    return _graphicsProvider->GetCurrentDeivce();
}

IGraphicsResourceManager* Engine::GetGraphicsResourceManager()
{
    return _graphicsProvider->GetGraphicsResourceManager();
}

IGameWindow* Engine::GetGameWindow()
{
    return _platformProvider->GetGameWindow();
}

IFileSystem* Engine::GetFileSystem()
{
    return _platformProvider->GetFileSystem();
}
TRV2_NAMESPACE_END