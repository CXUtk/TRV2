#include "TREngine.h"

#include <Utils/Logging/Logger.h>
#include <Configs/EngineSettings.h>
#include <Graphics/Renderers/SpriteRenderer.h>
#include <Core/Core_Interfaces.h>
#include <Core/Initializer/GLFWInitializer.h>
#include <Assets/AssetsManager.h>
#include <Utils/Structures/Rect.h>
#include <TRApplication.h>

TRV2_NAMESPACE_BEGIN
 TREngine::TREngine(int argc, char** argv, TRApplication* application)
 {
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

 TREngine::~TREngine()
 {
 }

 void TREngine::Run()
 {
     try
     {
         double minElapsedTime = 1.0 / _engineSettings->GetFPSCap();
         double prevTimestamp = GetGameTime();


         while (!_gameWindow->ShouldClose())
         {
             auto currentTime = GetGameTime();
             auto elapsed = currentTime - prevTimestamp;
             prevTimestamp = currentTime;

             _gameWindow->BeginFrame();
             {
                 _application->Update(elapsed);
                 _application->Draw(elapsed);
             }
             _gameWindow->EndFrame();

             _gameWindow->PollEvents();
             while (GetGameTime() - prevTimestamp < minElapsedTime)
             {
                 _gameWindow->PollEvents();
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

 double TREngine::GetGameTime() const
 {
     return _gameTimer->GetCurrentTime();
 }

 void TREngine::loadSupportiveSystem()
 {
 
 }

 void TREngine::loadGraphicsSystem()
 {
     _logger->LogInfo("Loading graphics system");

     // 这里 hard code 了一下OpenGL的API，因为第一阶段项目只考虑OpenGL
     GLFWInitializer intitializer(trv2::cptr(_engineSettings));

     _graphicsDevice = intitializer.GetGraphicsDevice();
     _gameWindow = intitializer.GetGameWindow();
     _gameTimer = intitializer.GetGameTimer();
 }


 void TREngine::loadLaunchSettings()
 {
     _logger->LogInfo("TR Engine Started");
     _engineSettings = std::make_shared<EngineSettings>();

     _logger->LogInfo("Current Engine Version: %s", _engineSettings->GetVersionString());
 }

 void TREngine::useApplication()
 {
     _application->Initialize(this);
 }

 void TREngine::SetApplication(TRApplication* application)
 {
     _application = application;
     useApplication();
 }

 std::shared_ptr<SpriteRenderer> TREngine::CreateSpriteRenderer() const
 {
     return std::make_shared<SpriteRenderer>(trv2::cptr(_graphicsDevice), _assetsManager->GetShader("builtin::sprite"),
         _assetsManager->GetTexture2D("builtin::sprite"));
 }

 void TREngine::loadResources()
 {
     _assetsManager = std::make_shared<AssetsManager>();
 }
TRV2_NAMESPACE_END