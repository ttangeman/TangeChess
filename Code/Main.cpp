#include "Chess.h"
#include "Chess.cpp"

MAIN_ENTRY_POINT()
{
    auto *pChess = new Tange::Chess("Chess", 600, 600);
    pChess->Run();
    delete pChess;

    return 0;
}