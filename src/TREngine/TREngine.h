#pragma once
#include <TREngine_Interfaces.h>
#include <memory>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
/**
 * @brief The main class of TREngine.
 * Provides a variety of interfaces, function classes for games
*/
class TREngine
{
public:
    TREngine();
    ~TREngine();

    void Initialize(int argc, char** argv);
    void SetApplication(TRApplication* application);

    void LoadAllAssets();
    void Run();

    double GetGameTime() const;

    /**
     * @brief Get the settings of engine.
    */
    const EngineSettings& GetEngineSetting() const
    {
        return trv2::cref(_engineSettings);
    }

    /**
     * @brief Get the graphics device.
    */
    const IGraphicsDevice& GetGraphicsDevice() const
    {
        return trv2::cref(_graphicsDevice);
    }

    /**
     * @brief Get the game window
    */
    const IGameWindow& GetGameWindow() const
    {
        return trv2::cref(_gameWindow);
    }

    const AssetsManager& GetAssetsManager() const
    {
        return trv2::cref(_assetsManager);
    }

    const Logger& GetLogger() const
    {
        return trv2::cref(_logger);
    }
private:
    // 加载函数
    void loadSupportiveSystem();
    void loadGraphicsSystem();
    void loadLaunchSettings();

    std::shared_ptr<EngineSettings> _engineSettings;
    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
    std::shared_ptr<IGameWindow> _gameWindow;
    std::shared_ptr<AssetsManager> _assetsManager;
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<IGameTimer> _gameTimer;

    TRApplication* _application;
};
TRV2_NAMESPACE_END