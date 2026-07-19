#include <cstdio>
#include <windows.h>
#include "methods/imethod.hpp"

extern IMosueMethod* g_postmessage;
extern IMosueMethod* g_setcursorpos;
extern IMosueMethod* g_mouseevent;
extern IMosueMethod* g_sendinput;
extern IMosueMethod* g_setcursorpos;

static void test(IMosueMethod* m, const char* name) {
    printf("--- %s ---\n", name);
    printf("  move_to(100,100): %s\n", m->move_to(100, 100) ? "ok" : "fail");
    printf("  move_relative(50,0): %s\n", m->move_relative(50, 0) ? "ok" : "fail");
}

int main() {
    test(g_setcursorpos, "SetCursorPos");
    Sleep(1000);
    test(g_mouseevent,    "mouse_event");
    Sleep(1000);
    test(g_sendinput,     "SendInput");
    Sleep(1000);
    test(g_postmessage,   "PostMessage");
    Sleep(1000);
    test(g_setcursorpos, "SetPhysicalCursorPos");
    return 0;
}