#pragma once
#include <TREngine_Interfaces.h>
#include <map>
#include <string>
#include <memory>
#include <Utils/Utils.h>
TRV2_NAMESPACE_BEGIN

template<typename _API>
class AssetsManager
{
public:
	using RawShaderType = typename _API::IRawShaderHandler;
	using ShaderProgramType = typename _API::IShaderProgramHandler;
	using TextureType = typename _API::ITexture2DHandler;
	AssetsManager()
	{
		loadBuiltinAssets();
	}

	ShaderProgramType* GetShader(const std::string& name) const
	{
		if (_shadersTable.find(name) == _shadersTable.end())
		{
			throw std::exception(string_format("Cannot find shader %s", name.c_str()).c_str());
		}
		return trv2::ptr(_shadersTable.at(name));
	}

	TextureType* GetTexture2D(const std::string& name) const
	{
		if (_texture2DTable.find(name) == _texture2DTable.end())
		{
			throw std::exception(string_format("Cannot find texture 2d %s", name.c_str()).c_str());
		}
		return trv2::ptr(_texture2DTable.at(name));
	}

private:
	std::map<std::string, std::shared_ptr<ShaderProgramType>> _shadersTable;
	std::map<std::string, std::shared_ptr<TextureType>> _texture2DTable;

	void loadBuiltinAssets()
	{
		auto vs = std::make_shared<RawShaderType>(ReadAllStringFromFile("Resources/Shaders/sprite2d.vert").c_str(), ShaderType::VERTEX_SHADER, "sprite2d.vert");
		auto fs = std::make_shared<RawShaderType>(ReadAllStringFromFile("Resources/Shaders/sprite2d.frag").c_str(), ShaderType::FRAGMENT_SHADER, "sprite2d.frag");
		_shadersTable["builtin::sprite"] = std::make_shared<ShaderProgramType>(vs, fs);

		auto gd = TREngine::GetInstance()->GetGraphicsDevice();
		int whitePixel = 0xffffffff;
		_texture2DTable["builtin::sprite"] = std::make_shared<TextureType>(gd, 1, 1, (unsigned char*)&whitePixel);
	}
	void loadTexture2D(){}
};
TRV2_NAMESPACE_END