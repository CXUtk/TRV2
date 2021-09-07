#pragma once
#include <TREngine_Interfaces.h>
#include <memory>
#include <glm/glm.hpp>
#include <Graphics/Shaders/OpenGLRawShader.h>
#include <Graphics/Shaders/OpenGLShaderProgram.h>
#include <Graphics/Textures/OpenGLTexture2D.h>
TRV2_NAMESPACE_BEGIN



/**
 * @brief The main class of TREngine.
 * Provides a variety of interfaces, function classes for games
*/
class TREngine
{
public:
    static TREngine* GetInstance() { return _instance; }
    TREngine(int argc, char** argv, TRApplication* application);
    ~TREngine();

    /**
     * @brief Manually set current application instance
     * @param application The application instance to run
    */
    void SetApplication(TRApplication* application);

    std::shared_ptr<SpriteRenderer> CreateSpriteRenderer();

    /**
     * @brief Run the application, or start the game
    */
    void Run();

    double GetGameTime() const;

    /**
     * @brief Get the settings of engine.
    */
    EngineSettings* GetEngineSetting()
    {
        return _engineSettings.get();
    }

    /**
     * @brief Get the graphics device.
    */
    IGraphicsDevice* GetGraphicsDevice()
    {
        return _graphicsDevice.get();
    }

    /**
     * @brief Get the game window
    */
    IGameWindow* GetGameWindow()
    {
        return _gameWindow.get();
    }
    
    /**
     * @brief Get the assets manager
    */
    AssetsManager<OpenGLAPI>* GetAssetsManager()
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
private:
    // 加载函数
    void loadSupportiveSystem();
    void loadGraphicsSystem();
    void loadLaunchSettings();
    void loadResources();
    void useApplication();

    std::shared_ptr<EngineSettings> _engineSettings;
    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
    std::shared_ptr<IGameWindow> _gameWindow;
    std::shared_ptr<AssetsManager<OpenGLAPI>> _assetsManager;
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<IGameTimer> _gameTimer;

    TRApplication* _application;

    static TREngine* _instance;
};
TRV2_NAMESPACE_END