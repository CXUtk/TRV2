#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <TREngine/TREngine_Interfaces.h>
#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/TRApplication.h>

class TRGame : public trv2::TRApplication
{
public:
    static TRGame& GetInstance();
    ~TRGame() override;

    virtual void Initialize(trv2::TREngine* engine) override;
    virtual void Update(double deltaTime) override;
    virtual void Draw(double deltaTime) override;
    virtual void Exit() override;

    const trv2::TREngine* GetEngine() const
    {
        return _engine;
    }

    const trv2::Logger& GetLogger() const
    {
        return trv2::cref(_logger);
    }

private:
    TRGame();

    // Other
    void logGameInfo();

    void loadGameContent();

    trv2::TREngine* _engine;
    std::shared_ptr<trv2::Logger> _logger;

    std::shared_ptr<trv2::ISpriteRenderer> _spriteRenderer;
    std::unique_ptr<GameWorld> _gameWorld;
    glm::vec2 _screenPosition;
};