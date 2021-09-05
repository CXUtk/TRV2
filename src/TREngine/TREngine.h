﻿#pragma once
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
    TREngine(int argc, char** argv, TRApplication* application);
    ~TREngine();

    void SetApplication(TRApplication* application);
    void Run();

    double GetGameTime() const;

    /**
     * @brief Get the settings of engine.
    */
    const EngineSettings* GetEngineSetting() const
    {
        return _engineSettings.get();
    }

    /**
     * @brief Get the graphics device.
    */
    const IGraphicsDevice* GetGraphicsDevice() const
    {
        return _graphicsDevice.get();
    }

    /**
     * @brief Get the game window
    */
    const IGameWindow* GetGameWindow() const
    {
        return _gameWindow.get();
    }

    const AssetsManager* GetAssetsManager() const
    {
        return _assetsManager.get();
    }

    const Logger* GetLogger() const
    {
        return _logger.get();
    }
private:
    // 加载函数
    void loadSupportiveSystem();
    void loadGraphicsSystem();
    void loadLaunchSettings();
    void useApplication();

    std::shared_ptr<EngineSettings> _engineSettings;
    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
    std::shared_ptr<IGameWindow> _gameWindow;
    std::shared_ptr<AssetsManager> _assetsManager;
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<IGameTimer> _gameTimer;

    TRApplication* _application;
};
TRV2_NAMESPACE_END