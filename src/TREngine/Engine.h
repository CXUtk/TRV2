#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>
#include <Core/Utils/GameTimer.h>

TRV2_NAMESPACE_BEGIN
/**
 * @brief brief The main class of TREngine. Provides a variety of interfaces, function classes for games.
 * @tparam _API Target graphics API
 * @tparam _Platform Target platform
*/
class Engine
{
public:

    Engine(int argc, char** argv, Application* application);
    ~Engine();

    /**
     * @brief Get the singleton instance of TREngine
     * @return
    */
    static Engine* GetInstance() { return _instance; }

    /**
     * @brief Manually set current application instance
     * @param application The application instance to run
    */
    void SetApplication(Application* application);

    /**
     * @brief Start game loop and application update and draw process
    */
    void Run();
   

    /**
     * @brief Get the settings of engine.
    */
    EngineSettings* GetEngineSetting() { return trv2::ptr(_engineSettings); }
    
    /**
     * @brief Get the assets manager
    */
    AssetsManager* GetAssetsManager() { return trv2::ptr(_assetsManager); }

    /**
     * @brief Get the instance of sprite renderer
     * @return
    */
    SpriteRenderer* GetSpriteRenderer() const { return trv2::ptr(_spriteRenderer); }

    /**
     * @brief Get logger
    */
    Logger* GetLogger() { return trv2::ptr(_logger); }

    /**
     * @brief Get graphics device
     * @return 
    */
    IGraphicsDevice* GetGraphicsDevice() { return _graphicsProvider->GetCurrentDeivce(); }

    /**
     * @brief Get graphics API resource manager
     * @return 
    */
    IGraphicsResourceManager* GetGraphicsResourceManager() { return _graphicsProvider->GetGraphicsResourceManager(); }

    /**
     * @brief Get game window
     * @return 
    */
    IGameWindow* GetGameWindow() { return trv2::ptr(_gameWindow); }

    /**
     * @brief Get input controller
     * @return 
    */
    InputController* GetInputController() { return trv2::ptr(_inputController); }


private:
    // 加载函数
    void loadSupportiveSystem();

    void loadGraphicsSystem();

    void loadLaunchSettings();

    void loadResources();

    void useApplication();


    std::shared_ptr<EngineSettings> _engineSettings;
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<SpriteRenderer> _spriteRenderer;
    std::shared_ptr<AssetsManager> _assetsManager;
    std::shared_ptr<InputController> _inputController;

    std::shared_ptr<IGraphicsProvider> _graphicsProvider;
    std::shared_ptr<IGameWindow> _gameWindow;

    GameTimer _gameTimer;

    Application* _application;

    static Engine* _instance;
};
TRV2_NAMESPACE_END