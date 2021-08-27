#pragma once
#include <TRV2.h>

#define ReadonlyProperty(type, publicName, privateName) public: type Get##publicName() { return privateName; } \
private: type ##privateName;

class TRGame {
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

    ReadonlyProperty(std::shared_ptr<ClientConfig>, ClientConfig, _clientConfig)
    ReadonlyProperty(std::shared_ptr<Logger>, Logger, _logger)

private:
    TRGame();

    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
};