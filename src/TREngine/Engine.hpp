#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <Engine_Interfaces.hpp>

#include <Utils/Logging/Logger.hpp>
#include <Configs/EngineSettings.hpp>
#include <Assets/AssetsManager.hpp>
#include <Graphics/Renderers/SpriteRenderer.hpp>

TRV2_NAMESPACE_BEGIN
/**
 * @brief brief The main class of TREngine. Provides a variety of interfaces, function classes for games.
 * @tparam _API Target graphics API
 * @tparam _Platform Target platform
*/
class Engine
{
public:

    /**
    * @brief Manually set current application instance
    * @param application The application instance to run
   */
    void SetApplication(Application* application)
    {
        _application = application;
        useApplication();
    }

    /**
     * @brief Get the singleton instance of TREngine
     * @return 
    */
    static TREngine* GetInstance() { return _instance; }

    /**
     * @brief Get the instance of sprite renderer
     * @return 
    */
    SpriteRenderer* GetSpriteRenderer() const
    {
        return _spriteRenderer.get();
    }

    /**
     * @brief Start game loop and application update and draw process
    */
    void Run()
    {
        try
        {
            double minElapsedTime = 1.0 / GetEngineSetting()->GetFPSCap();
            double prevTimestamp = GetGameTime();

            auto controller = GetInputController();
            auto gameWindow = GetGameWindow();

            while (!gameWindow->ShouldClose())
            {
                auto currentTime = GetGameTime();
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

                double t = GetGameTime() - prevTimestamp;
                //_logger->LogInfo("%lf, %d\n", t, (int)(1 / t));
                while (GetGameTime() - prevTimestamp < minElapsedTime)
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

    Engine(int argc, char** argv, TRApplication* application)
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

    ~Engine()
    {

    }

    /**
     * @brief Get current game time
     * @return 
    */
    double GetGameTime() const
    {
        return _platform->GetGameTimer()->GetCurrentTime();
    }

    /**
     * @brief Get the settings of engine.
    */
    EngineSettings* GetEngineSetting()
    {
        return _engineSettings.get();
    }
    
    /**
     * @brief Get the assets manager
    */
    AssetsManager* GetAssetsManager()
    {
        return _assetsManager.get();
    }

    /**
     * @brief Get logger
    */
    Logger* GetLogger()
    {
        return _logger.get();
    }

    /**
     * @brief Get graphics device
     * @return 
    */
    IGraphicsDevice* GetGraphicsDevice()
    {
        return _graphicsAPI->GetGraphicsDevice();
    }

    /**
     * @brief Get game window
     * @return 
    */
    IGameWindow* GetGameWindow()
    {
        return _platform->GetWindow();
    }

    /**
     * @brief Get input controller
     * @return 
    */
    IInputController* GetInputController()
    {
        return _platform->GetInputController();
    }

    /**
     * @brief Get game platform related timer
     * @return 
    */
    IGameTimer* GetGameTimer()
    {
        return _platform->GetGameTimer();
    }
private:
    // 加载函数
    void loadSupportiveSystem()
    {
        _logger->LogInfo("Loading platform specifics");
        _platform = std::make_shared<_Platform>(*_engineSettings);
    }

    void loadGraphicsSystem()
    {
        _logger->LogInfo("Loading graphics system");
        _graphicsAPI = std::make_shared<_API>(*_engineSettings);
    }

    void loadLaunchSettings()
    {
        _logger->LogInfo("TR Engine Started");
        _engineSettings = std::make_shared<EngineSettings>();

        _logger->LogInfo("Current Engine Version: %s", _engineSettings->GetVersionString());
    }

    void loadResources()
    {
        _assetsManager = std::make_shared<AssetsManager>();

        _spriteRenderer = std::make_shared<SpriteRenderer>(GetGraphicsDevice(), _assetsManager->GetShader("builtin::sprite"),
            _assetsManager->GetTexture2D("builtin::sprite"));
    }

    void useApplication()
    {
        assert(_application != nullptr);
        _application->Initialize(this);
    }

    std::shared_ptr<EngineSettings> _engineSettings;
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<SpriteRenderer> _spriteRenderer;
    std::shared_ptr<AssetsManager> _assetsManager;

    std::shared_ptr<IGraphicsProvider> _graphicsProvider;
    std::shared_ptr<IPlatformProvider> _platformProvider;

    Application* _application;

    static TREngine* _instance;
};


Engine* Engine::_instance = nullptr;
TRV2_NAMESPACE_END