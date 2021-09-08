#define _CRTDBG_MAP_ALLOC
#include <TRGame/TRGame.hpp>
#include <TREngine/TREngine.hpp>
#include <cstdio>

#include <Platform/GameTimer/GLFWGameTimer.hpp>
#include <Platform/GameWindow/GLFWGameWindow.hpp>
#include <Platform/InputController/GLFWInputController.hpp>

int main(int argc, char** argv)
{
    _CrtMemState sOld;
    _CrtMemState sNew;
    _CrtMemState sDiff;
    _CrtMemCheckpoint(&sOld); //take a snapshot
    try {
        TRGame game;
        trv2::IEngine engine(argc, argv, &game);
        engine.Run();
    }
    catch (std::exception ex) {
        fprintf(stderr, "Error encountered, game terminated!\n");
    }
    _CrtMemCheckpoint(&sNew); //take a snapshot 
    if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    {
        fprintf(stdout, "-----------_CrtMemDumpStatistics ---------\n");
        _CrtMemDumpStatistics(&sDiff);
        fprintf(stdout, "-----------_CrtMemDumpAllObjectsSince ---------\n");
        _CrtMemDumpAllObjectsSince(&sOld);
        fprintf(stdout, "-----------_CrtDumpMemoryLeaks ---------\n");
        _CrtDumpMemoryLeaks();
    }
    else
    {
        fprintf(stdout, "No memory leak\n");
    }
    return 0;
}
