#pragma once
#include "TREngine_Interfaces.h"
#include <Platform/GLFWPlatform.hpp>
#include <Graphics/OpenGLGraphicsAPI.hpp>

TRV2_NAMESPACE_BEGIN
template<typename _API, typename _Platform>
class TREngine;

using IEngine = TREngine<_G_API, _G_Platform>;

// Graphics resources
using IGraphicsDevice = typename _G_API::_GraphicsDevice_Type;
using IShaderProgram = typename _G_API::_ShaderProgram_Type;
using IRawShader = typename _G_API::_RawShader_Type;
using ITexture2D = typename _G_API::_Texture2D_Type;

// Platform utils
using IGameWindow = typename _G_Platform::_Window_Type;
using IInputController = typename _G_Platform::_InputController_Type;
using IGameTimer = typename _G_Platform::_GameTimer_Type;

// Handles
using IVertexBufferHandle = unsigned int;
using IShaderHandle = unsigned int;
using IShaderProgramHandle = unsigned int;
using ITextureHandle = unsigned int;
TRV2_NAMESPACE_END

