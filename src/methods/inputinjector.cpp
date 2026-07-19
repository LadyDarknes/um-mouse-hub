#include <windows.h>
#include <cstdio>
#include "imethod.hpp"

#if __has_include(<winrt/Windows.UI.Input.Preview.Injection.h>)
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Input.Preview.Injection.h>
#define HAS_INPUT_INJECTOR

using namespace winrt::Windows::UI::Input::Preview::Injection;

class InputInjectorMethod : public IMosueMethod {
private:
    InputInjector m_injector = nullptr;
    bool m_initialized = false;

    void ensure_initialized() {
        if (m_initialized) return;
        m_initialized = true;
        try {
            m_injector = InputInjector::TryCreate();
        } catch (...) {
            printf("InputInjectorMethod: Failed to initialize InputInjector.\n");
        }
    }

public:
    bool move_to(int x, int y) override {
        ensure_initialized();
        if (!m_injector) return false;

        try {
            InjectedInputMouseInfo mouseInfo;
            mouseInfo.MouseOptions(InjectedInputMouseOptions::Move | InjectedInputMouseOptions::Absolute);
            mouseInfo.DeltaX(x * 65535 / GetSystemMetrics(SM_CXSCREEN));
            mouseInfo.DeltaY(y * 65535 / GetSystemMetrics(SM_CYSCREEN));

            m_injector.InjectMouseInput({ mouseInfo });
            return true;
        } catch (...) {
            return false;
        }
    }

    bool move_relative(int dx, int dy) override {
        ensure_initialized();
        if (!m_injector) return false;

        try {
            InjectedInputMouseInfo mouseInfo;
            mouseInfo.MouseOptions(InjectedInputMouseOptions::Move);
            mouseInfo.DeltaX(dx);
            mouseInfo.DeltaY(dy);

            m_injector.InjectMouseInput({ mouseInfo });
            return true;
        } catch (...) {
            return false;
        }
    }
};

#else

class InputInjectorMethod : public IMosueMethod {
public:
    bool move_to(int x, int y) override {
        printf("InputInjectorMethod: Not supported on this compiler/system.\n");
        return false;
    }
    bool move_relative(int dx, int dy) override {
        printf("InputInjectorMethod: Not supported on this compiler/system.\n");
        return false;
    }
};

#endif

static InputInjectorMethod reg;
IMosueMethod* g_inputinjector = &reg;
