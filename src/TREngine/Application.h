#pragma once
#include <Core.h>

TRV2_NAMESPACE_BEGIN
/**
 * @brief The main application class used by TREngine.
 * This class if for game logic, you can implement them by inheriting this class.
*/
class Application {
public:
	virtual ~Application() = 0 {};

	/**
	 * @brief Initialization of the application after the engine is ready
	 * @param engine The instance of game engine
	*/
	virtual void Initialize(Engine* engine) = 0;

	/**
	 * @brief Specify how to update each frame, execute once per frame
	 * @param deltaTime How much time was elapsed from the last frame
	*/
	virtual void Update(double deltaTime) = 0;


	/**
	 * @brief Specify how to draw each frame, execute once per frame, after each Update
	 * @param deltaTime How much time was elapsed from the last frame
	*/
	virtual void Draw(double deltaTime) = 0;

	/**
	 * @brief Specify things to do when application should exit
	*/
	virtual void Exit() = 0;

	virtual bool ShouldSkipFrame(double elapsedFromFrameBegin) = 0;

	virtual void OnFrameEnd() = 0;
};
TRV2_NAMESPACE_END