#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <TREngine/TREngine_Interfaces.h>
#include <TRGame/TRGame_Interfaces.h>

class TRGame
{
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

    ReadonlyUniquePtrProperty(trv2::TREngine, Engine, engine);
    ReadonlyUniquePtrProperty(trv2::Logger, Logger, logger);
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