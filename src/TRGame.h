#pragma once
#include <TRV2.h>
#include <memory>

#define ReadonlyProperty(type, name) public: type Get##name() { return _##name; } \
private: type _##name;

class TRGame {
public:
    static TRGame& GetInstance();
    ~TRGame();

    void Initialize(int argc, char** argv);
    void Run();

    ReadonlyProperty(std::shared_ptr<ClientConfig>, clientConfig)

private:
    std::shared_ptr<IGraphicsDevice> _graphicsDevice;
};