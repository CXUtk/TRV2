﻿#pragma once
#include <TREngine_Interfaces.h>

TRV2_NAMESPACE_BEGIN
/**
 * @brief The main class of TREngine.
 * Provides a variety of interfaces, function classes for games
*/
class TRApplication {
public:
	virtual ~TRApplication() = 0 {};

	/**
	 * @brief Initialization of the application after the engine is ready
	*/
	virtual void Initialize(TREngine* engine) = 0;

	/**
	 * @brief Specify how to update each frame, execute once per frame
	*/
	virtual void Update(double deltaTime) = 0;

	/**
	 * @brief Specify how to draw each frame, execute once per frame, after each Update
	*/
	virtual void Draw(double deltaTime) = 0;

	/**
	 * @brief Specify things to do when application should exit
	*/
	virtual void Exit() = 0;
};
TRV2_NAMESPACE_END