#include <iostream>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <iomanip>

#pragma comment(lib, "Psapi.lib")

// Function to get CPU usage percentage
float get_cpu_usage() {
    FILETIME idle_time, kernel_time, user_time;
    static FILETIME prev_idle_time, prev_kernel_time, prev_user_time;
    if (GetSystemTimes(&idle_time, &kernel_time, &user_time)) {
        ULARGE_INTEGER idle, kernel, user;
        ULARGE_INTEGER prev_idle, prev_kernel, prev_user;
        idle.LowPart = idle_time.dwLowDateTime;
        idle.HighPart = idle_time.dwHighDateTime;
        kernel.LowPart = kernel_time.dwLowDateTime;
        kernel.HighPart = kernel_time.dwHighDateTime;
        user.LowPart = user_time.dwLowDateTime;
        user.HighPart = user_time.dwHighDateTime;
        prev_idle.LowPart = prev_idle_time.dwLowDateTime;
        prev_idle.HighPart = prev_idle_time.dwHighDateTime;
        prev_kernel.LowPart = prev_kernel_time.dwLowDateTime;
        prev_kernel.HighPart = prev_kernel_time.dwHighDateTime;
        prev_user.LowPart = prev_user_time.dwLowDateTime;
        prev_user.HighPart = prev_user_time.dwHighDateTime;

        ULONGLONG idle_diff = (idle.QuadPart - prev_idle.QuadPart);
        ULONGLONG kernel_diff = (kernel.QuadPart - prev_kernel.QuadPart);
        ULONGLONG user_diff = (user.QuadPart - prev_user.QuadPart);

        float cpu_usage = (kernel_diff + user_diff - idle_diff) * 100.0 / (kernel_diff + user_diff);
        prev_idle_time = idle_time;
        prev_kernel_time = kernel_time;
        prev_user_time = user_time;
        return cpu_usage;
    }
    return 0.0;
}

// Function to get RAM usage percentage
float get_ram_usage() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        return (float)(statex.dwMemoryLoad);
    }
    return 0.0;
}

float timeout_v1() {
    float cpu_usage = get_cpu_usage();
    float ram_usage = get_ram_usage();

    float cpu_usage_divided = cpu_usage / 100;
    float ram_usage_divided = ram_usage / 100;

    if (cpu_usage_divided < 0.1) {
        cpu_usage_divided *= 2;
    }
    if (ram_usage_divided < 0.1) {
        ram_usage_divided *= 2;
    }
    std::vector<float> data = {cpu_usage_divided, ram_usage_divided};
    float sum = 0;
    for (float value : data) {
        sum += value;
    }
    return round(sum / data.size() * 10) / 10;
}

float timeout_v2(DWORD pid = GetCurrentProcessId()) {
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (process == nullptr) {
        return 0.0;
    }

    FILETIME creation_time, exit_time, kernel_time, user_time;
    if (GetProcessTimes(process, &creation_time, &exit_time, &kernel_time, &user_time)) {
        // This example does not calculate process-specific CPU usage.
        // You would need to implement similar logic as `get_cpu_usage` for process CPU usage.
    }
    CloseHandle(process);

    float cpu_usage = get_cpu_usage();
    float ram_usage = 0.0;  // Implement specific logic for process RAM usage if needed

    float cpu_usage_divided = cpu_usage / 100;
    float ram_usage_divided = ram_usage / 100;

    if (cpu_usage_divided < 0.1) {
        cpu_usage_divided *= 2;
    }
    if (ram_usage_divided < 0.1) {
        ram_usage_divided *= 2;
    }
    std::vector<float> data = {cpu_usage_divided, ram_usage_divided};
    float sum = 0;
    for (float value : data) {
        sum += value;
    }
    return round(sum / data.size() * 10) / 10;
}
