#define NOMINMAX
#include <windows.h>
#include <setupapi.h>
#include <hidsdi.h>
#include "imethod.hpp"
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")

class HIDRawWriteMethod : public IMosueMethod {
private:
    HANDLE m_device = INVALID_HANDLE_VALUE;
    bool m_initialized = false;

    // Helper to find and open a suitable hid device (custom emulator Arduino/Teensy etc.)
    void ensure_initialized() {
        if (m_initialized) return;
        m_initialized = true;

        GUID hidGuid;
        HidD_GetHidGuid(&hidGuid);

        HDEVINFO hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
        if (hDevInfo == INVALID_HANDLE_VALUE) {
            return;
        }

        SP_DEVICE_INTERFACE_DATA devInterfaceData;
        devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

        for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &hidGuid, i, &devInterfaceData); ++i) {
            DWORD detailSize = 0;
            SetupDiGetDeviceInterfaceDetailA(hDevInfo, &devInterfaceData, NULL, 0, &detailSize, NULL);
            
            std::vector<BYTE> detailBuf(detailSize);
            PSP_DEVICE_INTERFACE_DETAIL_DATA_A devDetail = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_A>(detailBuf.data());
            devDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

            if (SetupDiGetDeviceInterfaceDetailA(hDevInfo, &devInterfaceData, devDetail, detailSize, NULL, NULL)) {
                std::string pathA = devDetail->DevicePath;
                for (auto& c : pathA) c = tolower(c);

                // We target common custom microcontrollers/mouse emulators.
                // Examples: Arduino Leonardo (VID 2341, PID 8036), Teensy (VID 16c0, PID 0486)
                bool is_target = (pathA.find("vid_2341") != std::string::npos) || (pathA.find("vid_16c0") != std::string::npos);

                if (!is_target) continue;

                HANDLE hDevice = CreateFileA(devDetail->DevicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
                
                if (hDevice != INVALID_HANDLE_VALUE) {
                    m_device = hDevice;
                    printf("HIDRawWriteMethod: Connected to HID device: %s\n", pathA.c_str());
                    break;
                }
            }
        }

        SetupDiDestroyDeviceInfoList(hDevInfo);

        if (m_device == INVALID_HANDLE_VALUE) {
            printf("HIDRawWriteMethod: No compatible writable HID device found.\n");
        }
    }

    bool send_report(int buttons, int dx, int dy, int wheel) {
        ensure_initialized();
        if (m_device == INVALID_HANDLE_VALUE) return false;

        // Standard 5 byte mouse hid report:
        // Byte 0: Report ID (usually 0 if not used, or 1)
        // Byte 1: Buttons (bit 0 = left, bit 1 = right, bit 2 = middle)
        // Byte 2: X movement (-127 to 127)
        // Byte 3: Y movement (-127 to 127)
        // Byte 4: Wheel (-127 to 127)
        unsigned char report[5];
        report[0] = 0x01; // Report ID 1 (standard for many HID devices)
        report[1] = static_cast<unsigned char>(buttons);
        report[2] = static_cast<unsigned char>(std::max(-127, std::min(127, dx)));
        report[3] = static_cast<unsigned char>(std::max(-127, std::min(127, dy)));
        report[4] = static_cast<unsigned char>(std::max(-127, std::min(127, wheel)));

        DWORD bytesWritten = 0;
        BOOL ok = WriteFile(m_device, report, sizeof(report), &bytesWritten, NULL);
        return ok && (bytesWritten == sizeof(report));
    }

public:
    ~HIDRawWriteMethod() {
        if (m_device != INVALID_HANDLE_VALUE) {
            CloseHandle(m_device);
        }
    }

    bool move_to(int x, int y) override {
        POINT p;
        if (GetCursorPos(&p)) {
            return move_relative(x - p.x, y - p.y);
        }
        return false;
    }

    bool move_relative(int dx, int dy) override {
        // Send movement in chunks since HID report deltas are limited to [-127, 127]
        bool success = true;
        while (dx != 0 || dy != 0) {
            int stepX = std::max(-127, std::min(127, dx));
            int stepY = std::max(-127, std::min(127, dy));
            
            if (!send_report(0, stepX, stepY, 0)) {
                success = false;
                break;
            }
            dx -= stepX;
            dy -= stepY;
            Sleep(5);
        }
        return success;
    }
};

static HIDRawWriteMethod reg;
IMosueMethod* g_hidrawwrite = &reg;
