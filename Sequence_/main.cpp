#include "game.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    // Card suits are printed as UTF-8 symbols; the Windows console needs to
    // be told to expect UTF-8 or they show up as garbage like "â™¥"
    SetConsoleOutputCP(CP_UTF8);
#endif

    Game game;
    game.run();
    return 0;
}
