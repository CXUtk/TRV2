#pragma once
#include <TRV2.h>
#include <memory>

#define ReadonlyProperty(type, publicName, privateName) public: type Get##publicName() { return privateName; } \
private: type ##privateName;

class TRGame {
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

    ReadonlyProperty(std::shared_ptr<ClientConfig>, ClientConfig, _clientConfig)

private:
    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
};