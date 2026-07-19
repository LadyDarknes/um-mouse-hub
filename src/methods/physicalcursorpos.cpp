#include <windows.h>
#include "imethod.hpp"

class PhysicalCursorPosMethod : public IMosueMethod {
    bool move_to(int x, int y) override {
        return SetPhysicalCursorPos(x, y) != 0;
    }
    bool move_relative(int dx, int dy) override {
        POINT p;
        GetPhysicalCursorPos(&p);
        return SetPhysicalCursorPos(p.x + dx, p.y + dy) != 0;
    }
};

static PhysicalCursorPosMethod reg;
IMosueMethod* g_physicalcursorpos = &reg;