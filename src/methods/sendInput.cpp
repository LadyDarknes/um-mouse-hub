#include <windows.h>
#include "imethod.hpp"

class SendInputMethod : public IMosueMethod {
    bool move_to(int x, int y) override {
        INPUT input[1] = {}; // okay 1 input 1 array, but why is it an array? well, because SendInput takes an array of inputs, so I just made it an array of 1 input ¯\_(ツ)_/¯
        input[0].type = INPUT_MOUSE;
        input[0].mi.dx = x * 65535 / GetSystemMetrics(SM_CXSCREEN);
        input[0].mi.dy = y * 65535 / GetSystemMetrics(SM_CYSCREEN);
        input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE; // I didnt realise until I check, this mf is mouse event xd
        SendInput(1, input, sizeof(INPUT));
        return true;
    }
    bool move_relative(int dx, int dy) override {
        INPUT input[2] = {};
        input[0].type = INPUT_MOUSE;
        input[0].mi.dx = dx;
        input[0].mi.dy = dy;
        input[0].mi.dwFlags = MOUSEEVENTF_MOVE;
        SendInput(1, input, sizeof(INPUT));
        return true;
    }
};


static SendInputMethod reg;
IMosueMethod* g_sendinput = &reg;
