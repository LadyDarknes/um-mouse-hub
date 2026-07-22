#include <windows.h>
#include "imethod.hpp"

class SendMessageMethod : public IMosueMethod {
    bool move_to(int x, int y) override {
        LPARAM lParam = MAKELPARAM(x, y);
        SendMessage(HWND_BROADCAST, WM_MOUSEMOVE, 0, lParam);
        return true;
    }
    bool move_relative(int dx, int dy) override {
        LPARAM lParam = MAKELPARAM(dx, dy);
        SendMessage(HWND_BROADCAST, WM_MOUSEMOVE, 0, lParam);
        return true;
    }
};


