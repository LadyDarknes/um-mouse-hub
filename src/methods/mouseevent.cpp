#include <windows.h>
#include "imethod.hpp"

class MouseEventMethod : public IMosueMethod {
    bool move_to(int x, int y) override {
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
        return true;
    }
    bool move_relative(int dx, int dy) override {
        mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);
        return true;
    }
};

static MouseEventMethod reg;
IMosueMethod* g_mouseevent = &reg;
