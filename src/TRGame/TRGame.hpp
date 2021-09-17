#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <TREngine/Core.h>

#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/Application.h>
#include <TREngine/Core/Structures/Rect.hpp>


class TRGame : public trv2::Application
{
public:
    static TRGame* GetInstance() { return _instance; }
    TRGame();
    ~TRGame() override;

    virtual void Initialize(trv2::Engine* engine) override;
    virtual void Update(double deltaTime) override;
    virtual void Draw(double deltaTime) override;
    virtual void Exit() override;

    trv2::Engine* GetEngine()
    {
        return _engine;
    }

    trv2::Logger* GetLogger() const
    {
        return trv2::ptr(_logger);
    }

    GameWorld* GetGameWorld() const
    {
        return trv2::ptr(_gameWorld);
    }

    Player* GetLocalPlayer() const
    {
        return trv2::ptr(_mainPlayer);
    }

private:
    // Other
    void logGameInfo();
    void loadGameContent();

    trv2::Engine* _engine;
    std::shared_ptr<trv2::Logger> _logger;

    trv2::SpriteRenderer* _spriteRenderer;
    std::unique_ptr<GameWorld> _gameWorld;
    std::unique_ptr<Player> _mainPlayer;
    
    trv2::Rect2D<float> _screenRect;
    glm::mat4 _projection;
    float _expScale;
    glm::vec2 _mouseDragStart;
    glm::vec2 _oldScreenPos;


    std::shared_ptr<trv2::RenderTarget2D> _tileTarget;
    std::shared_ptr<trv2::RenderTarget2D> _shadowMapSwap[2];

    static TRGame* _instance;
};