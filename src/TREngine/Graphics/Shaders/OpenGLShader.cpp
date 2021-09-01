#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

TRV2_NAMESPACE_BEGIN
void OpenGLShader::Apply() {
	glUseProgram(_id);
}

void OpenGLShader::SetParameteri1(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(this->_id, name.c_str()), value);
}

void OpenGLShader::SetParameterfv2(const std::string& name, glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(this->_id, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::SetParameterfv4(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(this->_id, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::SetParameterfm4x4(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
TRV2_NAMESPACE_END