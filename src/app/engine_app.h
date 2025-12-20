#ifndef GAME_APP_H
#define GAME_APP_H

#include "../engine/engine_gui.h"
#include "engine/engine.h"

class GameApp {
public:
    GameApp();
    void run();

private:
    Engine game_;
    TestGUI gui_;
};

#endif