#pragma once
#include <TRV2.h>
#include <memory>

class TRGame {
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

    ReadonlyProperty(std::shared_ptr<ClientConfig>, ClientConfig, clientConfig)
    ReadonlyProperty(std::shared_ptr<Logger>, Logger, logger)

private:
    TRGame();
    void logTRHeaderInfos();


    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
};