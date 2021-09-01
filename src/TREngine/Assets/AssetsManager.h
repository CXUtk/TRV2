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

private:
	void loadBuiltinAssets();
	std::map<std::string, std::shared_ptr<IShader>> _shadersTable;
};
TRV2_NAMESPACE_END