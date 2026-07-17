#include <windows.h>
#include "imethod.hpp"

class PostMessageMethod : public IMosueMethod {
    bool move_to(int x, int y) override {
        LPARAM lParam = MAKELPARAM(x, y);
        PostMessage(HWND_BROADCAST, WM_MOUSEMOVE, 0, lParam);
        return true;
    }
    bool move_relative(int dx, int dy) override {
        LPARAM lParam = MAKELPARAM(dx, dy);
        PostMessage(HWND_BROADCAST, WM_MOUSEMOVE, 0, lParam);
        return true;
    }
};

static PostMessageMethod reg;
IMosueMethod* g_postmessage = &reg;
