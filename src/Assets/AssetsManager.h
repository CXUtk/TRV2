﻿#pragma once
#include <TRV2.h>
#include <map>
#include <string>
#include <memory>

class AssetsManager
{
public:
	AssetsManager();
	
	std::shared_ptr<OpenGLShader> GetShader(const std::string& name) const;

private:
	std::map<std::string, std::shared_ptr<OpenGLShader>> _shadersTable;
};
