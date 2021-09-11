#pragma once
#include <map>
#include <string>
#include <memory>
#include <Core.h>

TRV2_NAMESPACE_BEGIN

class AssetsManager
{
public:
	AssetsManager(IGraphicsResourceManager* resourceManager);

	ShaderProgram* GetShader(const std::string& name) const;
	Texture2D* GetTexture2D(const std::string& name) const;

private:
	std::map<std::string, std::shared_ptr<ShaderProgram>> _shadersTable;
	std::map<std::string, std::shared_ptr<Texture2D>> _texture2DTable;

	void loadBuiltinAssets();
	void loadTexture2D();

	IGraphicsResourceManager* _resourceManager;
};
TRV2_NAMESPACE_END