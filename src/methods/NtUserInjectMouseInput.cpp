#include <windows.h>
#include "imethod.hpp"

/*
 * NOTE:
 * InjectMouseInput API is using (user32.dll) MOUSEINPUT for relative movement and (win32u.dll) InjectedInputMouseInfo for absolute movement
 * 
 * (Flags):
 *   0x0001 = MOUSEEVENTF_MOVE (Relative Move)
 *   0x0002 = MOUSEEVENTF_LEFTDOWN (irs funny I realise it falsely xd)
 *   0x0004 = MOUSEEVENTF_LEFTUP
 *   0x0008 = MOUSEEVENTF_RIGHTDOWN
 *   0x0010 = MOUSEEVENTF_RIGHTUP
 *   0x8001 = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE (Absolute Move)
 */

typedef struct _InjectedInputMouseInfo {
    LONG DeltaX;
    LONG DeltaY;
    DWORD MouseData;
    DWORD MouseOptions; // 0x0001 = MOUSEEVENTF_MOVE
    DWORD TimeStamp;
    ULONG_PTR ExtraInfo;
} InjectedInputMouseInfo, *PInjectedInputMouseInfo;

typedef BOOL (WINAPI *InjectMouseInput_t)(const InjectedInputMouseInfo* pInputs, UINT cInputs);
typedef BOOL (WINAPI *InitializeMouseInjection_t)();

class NtUserInjectMethod : public IMosueMethod {
    InjectMouseInput_t InjectMouseInput = nullptr;

public:
    NtUserInjectMethod() {
        HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
        if (hUser32) {
            InitializeMouseInjection_t fnInit = (InitializeMouseInjection_t)GetProcAddress(hUser32, "InitializeMouseInjection");
            if (fnInit) {
                fnInit();
            }
            InjectMouseInput = (InjectMouseInput_t)GetProcAddress(hUser32, "InjectMouseInput");
        }
        if (!InjectMouseInput) {
            HMODULE hWin32u = GetModuleHandleW(L"win32u.dll");
            if (!hWin32u) hWin32u = LoadLibraryW(L"win32u.dll");
            if (hWin32u) {
                InjectMouseInput = (InjectMouseInput_t)GetProcAddress(hWin32u, "NtUserInjectMouseInput");
            }
        }
    }

    bool move_relative(int dx, int dy) override {
        if (!InjectMouseInput) return false;

        InjectedInputMouseInfo info = {};
        info.DeltaX = dx;
        info.DeltaY = dy;
        info.MouseOptions = 0x0001;

        return InjectMouseInput(&info, 1) != 0;
    }

    bool move_to(int x, int y) override {
        POINT p;
        if (!GetCursorPos(&p)) return false;
        return move_relative(x - p.x, y - p.y);
    }
};

static NtUserInjectMethod reg;
IMosueMethod* g_ntuserinject = &reg;
