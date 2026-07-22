#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>
#include <cstdio>
#include "imethod.hpp"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class DirectInputMethod : public IMosueMethod {
    IDirectInput8* pDI = nullptr;
    IDirectInputDevice8* pMouse = nullptr;

public:
    DirectInputMethod(){
        DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL);
        if (pDI) {
            pDI->CreateDevice(GUID_SysMouse, &pMouse, NULL);
            if (pMouse) {
                pMouse->SetDataFormat(&c_dfDIMouse);
                pMouse->SetCooperativeLevel(GetForegroundWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
                pMouse->Acquire();
            }
        }
    }

    ~DirectInputMethod() {
        if (pMouse) {
            pMouse->Unacquire();
            pMouse->Release();
        }
        if (pDI) {
            pDI->Release();
        }
    }

    bool move_relative(int dx, int dy) override {
        if (!pMouse) return false;
        DIDEVICEOBJECTDATA od[2] = {};
        DWORD dwInOut = 0;
        if (dx != 0) {
            od[dwInOut].dwOfs = DIMOFS_X;
            od[dwInOut].dwData = static_cast<DWORD>(dx);
            dwInOut++;
        }
        if (dy != 0) {
            od[dwInOut].dwOfs = DIMOFS_Y;
            od[dwInOut].dwData = static_cast<DWORD>(dy);
            dwInOut++;
        }
        if (dwInOut == 0) return true;
        HRESULT hr = pMouse->SendDeviceData(sizeof(DIDEVICEOBJECTDATA), od, &dwInOut, 0);
        return SUCCEEDED(hr);
    }

    bool move_to(int x, int y) override {
        POINT p;
        GetCursorPos(&p);
        return move_relative(x - p.x, y - p.y);
    }
};

static DirectInputMethod reg;
IMosueMethod* g_directinput = &reg;