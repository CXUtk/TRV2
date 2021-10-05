#pragma once
#include <Core.h>
#include <string>

TRV2_NAMESPACE_BEGIN
class EngineSettings {
public:
	EngineSettings();

	/**
	 * @brief If the window size can be changed by user
	 * @return
	*/
	bool IsWindowResizable() const {
		return _windowResizable;
	}

	/**
	 * @brief Get the title string of the startup game window
	 * @return
	*/
	const char* GetWindowTitle() const {
		return _windowTitle.c_str();
	}

	/**
	 * @brief Get the version string of the engine
	 * @return
	*/
	const char* GetVersionString() const {
		return _versionString.c_str();
	}

	/**
	 * @brief Get the width of the startup game window
	 * @return
	*/
	int GetWindowWidth() const {
		return _windowWidth;
	}

	/**
	 * @brief Get the height of the startup game window
	 * @return
	*/
	int GetWindowHeight() const {
		return _windowHeight;
	}

	/**
	 * @brief Get the maximum frames per second of the game
	 * @return
	*/
	int GetFPSCap() const {
		return _fpsCap;
	}



private:
	// Client properties
	bool _windowResizable;
	std::string _windowTitle;
	std::string _versionString;

	int _windowWidth, _windowHeight;
	int _fpsCap;
};
TRV2_NAMESPACE_END