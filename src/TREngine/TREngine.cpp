#include "TREngine.h"

#include <Utils/Logging/Logger.h>
#include <Configs/EngineSettings.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/TROpenGLAPI.h>
#include <Core/Interfaces/ITRWindow.h>
#include <Assets/AssetsManager.h>
#include <Utils/Structures/Rect.h>

TRV2_NAMESPACE_BEGIN
 TREngine::TREngine()
 {
 }

 TREngine::~TREngine()
 {
 }

 void TREngine::Initialize(int argc, char** argv)
 {
     _logger = std::make_unique<Logger>();
     _engineSetting = std::make_shared<EngineSettings>();
     try {
         loadSupportiveSystem();
         loadGraphicsSystem();
     }
     catch (std::exception ex) {
         _logger->LogError("Error: %s", ex.what());
         throw;
     }
 }

 void TREngine::LoadAllAssets()
 {
 }

 double TREngine::GetGameTime()
 {
     return _apiUtils->GetTime();
 }

 void TREngine::loadSupportiveSystem()
 {
     logEngineInfo();
 }

 void TREngine::loadGraphicsSystem()
 {
     _logger->LogInfo("Loading graphics system");

     // 这里 hard code 了一下OpenGL的API，因为第一阶段项目只考虑OpenGL
     TROpenGLAPIGenerator apiGenerator;
     apiGenerator.Initialize(cref(_engineSetting));

     _graphicsDevice = apiGenerator.CreateGraphicsDevice();
     _gameWindow = apiGenerator.CreateWindow();
     _apiUtils = apiGenerator.CreateAPIUtils();

     _assetsManager = std::make_shared<AssetsManager>();
     _spriteRenderer = _graphicsDevice->CreateSpriteRenderer();
 }


 void TREngine::logEngineInfo()
 {
     _logger->LogInfo("TR Engine Started");
     auto version = _engineSetting->GetVersionString();
     _logger->LogInfo("Current Engine Version: %s", version.c_str());
 }
TRV2_NAMESPACE_END