#pragma once
#include <TREngine_Interfaces.h>
#include <memory>
#include <glm/glm.hpp>

class TRGame
{
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

    ReadonlyUniquePtrProperty(TREngine, Engine, engine);
    ReadonlyUniquePtrProperty(Logger, Logger, logger);
private:
    TRGame();

    // 核心流程
    void update();
    void draw();


    // Other
    void logGameInfo();

    void loadEngine(int argc, char** argv);
    void loadGameContent();

    std::unique_ptr<GameWorld> _gameWorld;
    glm::vec2 _screenPosition;
};