//#pragma once
//
//#include <bitset>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//
//#include <Core.h>
//#include <Platform/Platform_Interfaces.h>
//#include <TREngine/Configs/EngineSettings.h>
//
//TRV2_NAMESPACE_BEGIN
//class GLFWInputController
//{
//public:
//	GLFWInputController(GLFWwindow* window, const EngineSettings& settings);
//	~GLFWInputController();
//
//	
//
//private:
//	GLFWwindow* _window;
//
//	// Keys
//	std::bitset<(int)TRV2KeyCode::__COUNT> _curKeysDown;
//	std::bitset<(int)TRV2KeyCode::__COUNT> _wasKeysDown;
//
//	// Mouse wheel
//	glm::vec2 _scrollWheel;
//
//	// Mouse button
//	std::bitset<(int)TRV2MouseButtonCode::__COUNT> _curMouseButtonDown;
//	std::bitset<(int)TRV2MouseButtonCode::__COUNT> _wasMouseButtonDown;
//
//	void initializeCodeMapping();
//};
//TRV2_NAMESPACE_END