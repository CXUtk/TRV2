#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <TREngine/TREngine_Interfaces.h>
#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/TRApplication.h>
#include <TREngine/Utils/Structures/Rect.h>

class TRGame : public trv2::TRApplication
{
public:
    static TRGame* GetInstance() { return _instance; }
    TRGame();
    ~TRGame() override;

    virtual void Initialize(trv2::TREngine* engine) override;
    virtual void Update(double deltaTime) override;
    virtual void Draw(double deltaTime) override;
    virtual void Exit() override;

    const trv2::TREngine* GetEngine() const
    {
        return _engine;
    }

    const trv2::Logger* GetLogger() const
    {
        return _logger.get();
    }

private:
    // Other
    void logGameInfo();

    void loadGameContent();


    trv2::TREngine* _engine;
    std::shared_ptr<trv2::Logger> _logger;

    std::shared_ptr<trv2::SpriteRenderer> _spriteRenderer;
    std::unique_ptr<GameWorld> _gameWorld;
    
    trv2::Rect _screenRect;
    glm::mat4 _projection;
    float _expScale;
    glm::vec2 _mouseDragStart;
    glm::vec2 _oldScreenPos;


    static TRGame* _instance;
};