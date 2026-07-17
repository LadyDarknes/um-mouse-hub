#include <windows.h>
#include "imethod.hpp"

class SetCursorPosMethod : public IMosueMethod {
    bool move_to(int x, int y) override {
        return SetCursorPos(x, y) != 0;
    }
    bool move_relative(int dx, int dy) override {
        POINT p;
        GetCursorPos(&p);
        return SetCursorPos(p.x + dx, p.y + dy) != 0;
    }
};

static SetCursorPosMethod reg;
IMosueMethod* g_setcursorpos = &reg;
