#include "TRGame.h"

TRGame& TRGame::GetInstance() {
    static TRGame game;
    return game;
}

TRGame::~TRGame() {
}

void TRGame::Initialize() {
}

void TRGame::Run() {
}
