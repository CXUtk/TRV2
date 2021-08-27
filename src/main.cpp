﻿#include <TRV2.h>
#include <cstdio>
#include <TRGame.h>


int main(int argc, char** argv)
{
    try {
        auto& game = TRGame::GetInstance();
        game.Initialize(argc, argv);
        game.Run();
    }
    catch (std::exception ex) {
        fprintf(stderr, "ERROR: %s", ex.what());
    }
    return 0;
}