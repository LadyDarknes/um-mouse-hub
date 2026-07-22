#include <cstdio>
#include <windows.h>
#if __has_include(<winrt/Windows.Foundation.h>)
#include <winrt/Windows.Foundation.h>
#define HAS_WINRT
#endif
#include "methods/imethod.hpp"

extern IMosueMethod* g_postmessage;
extern IMosueMethod* g_sendMessage;
extern IMosueMethod* g_setcursorpos;
extern IMosueMethod* g_mouseevent;
extern IMosueMethod* g_sendinput;
extern IMosueMethod* g_physicalcursorpos;
extern IMosueMethod* g_inputinjector;
extern IMosueMethod* g_hidrawwrite;



static void test(IMosueMethod* m, const char* name) {
    printf("--- %s ---\n", name);
    if (!m) {
        printf("  [Error: Method pointer is null]\n");
        return;
    }
    printf("  move_to(100,100): %s\n", m->move_to(100, 100) ? "ok" : "fail");
    printf("  move_relative(50,0): %s\n", m->move_relative(50, 0) ? "ok" : "fail");
}

int main() {
#ifdef HAS_WINRT
    try {
        winrt::init_apartment();
    } catch (...) {
        printf("Failed to initialize Windows Runtime (WinRT).\n");
    }
#endif

    test(g_setcursorpos,       "SetCursorPos");
    Sleep(1000);
    test(g_mouseevent,         "mouse_event");
    Sleep(1000);
    test(g_sendinput,          "SendInput");
    Sleep(1000);
    test(g_postmessage,        "PostMessage");
    Sleep(1000);
    test(g_physicalcursorpos,  "SetPhysicalCursorPos");
    Sleep(1000);
    test(g_inputinjector,      "InputInjector");
    Sleep(1000);
    test(g_hidrawwrite,        "HIDRawWrite");
    Sleep(1000);
    test(g_sendMessage,         "SendMessage");
    
    return 0;
}