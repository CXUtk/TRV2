#pragma once
#include <TREngine_Interfaces.h>
#include <map>
#include <string>
#include <memory>
TRV2_NAMESPACE_BEGIN
class AssetsManager
{
public:
	AssetsManager();
	
	std::shared_ptr<IShader> GetShader(const std::string& name) const;
	std::shared_ptr<ITexture2D> GetTexture2D(const std::string& name) const;

private:
	std::map<std::string, std::shared_ptr<IShader>> _shadersTable;
	std::map<std::string, std::shared_ptr<ITexture2D>> _texture2DTable;

	void loadBuiltinAssets();
	void loadTexture2D();
};
TRV2_NAMESPACE_END