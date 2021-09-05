#include <TRGame/TRGame.h>
#include <TREngine/TREngine.h>
#include <cstdio>

int main(int argc, char** argv)
{
    try {
        trv2::TREngine engine(argc, argv, &TRGame::GetInstance());
        engine.Run();
    }
    catch (std::exception ex) {
        fprintf(stderr, "Error encountered, game terminated!\n");
    }
    return 0;
}
