#pragma once
#include <Interfaces.h>
#include <map>
#include <string>
#include <memory>

class AssetsManager
{
public:
	AssetsManager();
	
	std::shared_ptr<OpenGLShader> GetShader(const std::string& name) const;

private:
	void loadBuiltinAssets();
	std::map<std::string, std::shared_ptr<OpenGLShader>> _shadersTable;
};
