#include <windows.h>
#include "winternl.h"
#include "imethod.hpp"

typedef NTSTATUS (WINAPI *NtUserSetCursorPos_t)(int x, int y);
typedef NTSTATUS (WINAPI *NtUserGetCursorPos_t)(UINT cInputs, LPINPUT pInputs, int cbSize);

class Win32uSyscallMethod : public IMosueMethod {
    NtUserSetCursorPos_t fnSetCursorPos = nullptr;
    NtUserGetCursorPos_t fnSendInput = nullptr;

public:
    Win32uSyscallMethod() {
        HMODULE hWin32u = GetModuleHandleW(L"win32u.dll");
        if (!hWin32u) hWin32u = LoadLibraryW(L"win32u.dll"); // Attempt to load if not already loaded, idk but ai suggested this, so I added it.
        if (hWin32u) {
            fnSetCursorPos = (NtUserSetCursorPos_t)GetProcAddress(hWin32u, "NtUserSetCursorPos");
            fnSendInput = (NtUserGetCursorPos_t)GetProcAddress(hWin32u, "NtUserSendInput");
        }
    }

    bool move_to(int x, int y) override {
        if (fnSetCursorPos) {
            return fnSetCursorPos(x, y) != 0;
        }
        return false;
    }

    bool move_relative(int dx, int dy) override {
        if (fnSendInput) {
            INPUT input = {};
            input.type = INPUT_MOUSE;
            input.mi.dx = dx;
            input.mi.dy = dy;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            UINT count = fnSendInput(1, &input, sizeof(INPUT));
            return count == 1;
        }
        return false;
    }
};

static Win32uSyscallMethod reg;
IMosueMethod* g_win32u_syscall = &reg;