#pragma once
#include <TRV2.h>
#include <memory>

class TRGame {
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

private:
    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
};