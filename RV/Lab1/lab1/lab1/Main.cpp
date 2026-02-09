#include <windows.h>
#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

constexpr double X0 = 1.0;
constexpr double X1 = 2.0;
constexpr double DX = 1e-7;
const long long N_STEPS = static_cast<long long>((X1 - X0) / DX + 0.5);
const long long TOTAL_POINTS = N_STEPS + 1;

constexpr int WINDOW_MS = 100;
constexpr int CHUNK_SIZE = 500;

std::atomic<long long> global_index{ 0 };
double global_sum = 0.0;
std::mutex sum_mutex;

double total_cpu_ms = 0.0;
double total_sleep_ms = 0.0;
std::mutex time_mutex;

inline unsigned long long FileTimeToUll(const FILETIME& ft) {
    return (static_cast<unsigned long long>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

struct ThreadParam {
    int desired_load_percent;
};

DWORD WINAPI WorkerThread(LPVOID lpParam) {
    ThreadParam* tp = reinterpret_cast<ThreadParam*>(lpParam);
    int load = tp->desired_load_percent;
    double target_cpu_ms = WINDOW_MS * (load / 100.0);

    double cpu_accum_ms = 0.0;
    HANDLE hThread = GetCurrentThread();

    FILETIME ct{}, et{}, kt{}, ut{};
    unsigned long long prev_cpu_100ns = 0;
    if (GetThreadTimes(hThread, &ct, &et, &kt, &ut)) {
        prev_cpu_100ns = FileTimeToUll(kt) + FileTimeToUll(ut);
    }

    while (true) {
        long long start = global_index.fetch_add(CHUNK_SIZE);
        if (start >= TOTAL_POINTS) break;
        long long end = start + CHUNK_SIZE;
        if (end > TOTAL_POINTS) end = TOTAL_POINTS;

        // Вычисление интегралла (метод трапеций)
        double local_sum = 0.0;
        for (long long i = start; i < end; ++i) {
            double x = X0 + i * DX;
            double fx = std::cos(x * x);
            double weight = (i == 0 || i == N_STEPS) ? 0.5 : 1.0;
            local_sum += weight * fx;
        }
        local_sum *= DX;

        {
            std::lock_guard<std::mutex> lock(sum_mutex);
            global_sum += local_sum;
        }

        // Измеряем CPU время с момента последнего замера
        if (GetThreadTimes(hThread, &ct, &et, &kt, &ut)) {
            unsigned long long curr_cpu_100ns = FileTimeToUll(kt) + FileTimeToUll(ut);
            unsigned long long delta100ns = (curr_cpu_100ns > prev_cpu_100ns) ? (curr_cpu_100ns - prev_cpu_100ns) : 0;
            prev_cpu_100ns = curr_cpu_100ns;

            double delta_cpu_ms = static_cast<double>(delta100ns) / 10000.0;
            cpu_accum_ms += delta_cpu_ms;

            {
                std::lock_guard<std::mutex> tlock(time_mutex);
                total_cpu_ms += delta_cpu_ms;
            }
        }

        // Управление нагрузкой
        if (load < 100 && cpu_accum_ms >= target_cpu_ms) {
            double requested_sleep_ms = WINDOW_MS - cpu_accum_ms;
            if (requested_sleep_ms < 0.0) requested_sleep_ms = 0.0;

            DWORD req = static_cast<DWORD>(requested_sleep_ms + 0.5);
            if (req == 0 && requested_sleep_ms > 0.001) req = 1;

            auto t0 = std::chrono::high_resolution_clock::now();
            if (req > 0) Sleep(req);
            auto t1 = std::chrono::high_resolution_clock::now();

            double actual_sleep = std::chrono::duration<double, std::milli>(t1 - t0).count();
            {
                std::lock_guard<std::mutex> tlock(time_mutex);
                total_sleep_ms += actual_sleep;
            }

            cpu_accum_ms -= target_cpu_ms;
            if (cpu_accum_ms < 0) cpu_accum_ms = 0;
        }
    }

    return 0;
}

static inline std::string trim(const std::string& s) {
    const std::string ws = " \t\n\r\f\v";
    size_t l = s.find_first_not_of(ws);
    if (l == std::string::npos) return std::string();
    size_t r = s.find_last_not_of(ws);
    return s.substr(l, r - l + 1);
}

int main(int argc, char** argv) {
    timeBeginPeriod(1); // Устанавливаем разрешение системного таймера 1 мс(вместо стандартных 15.6)

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int desired_load = 100;
    if (argc >= 2) {
        desired_load = std::atoi(argv[1]);
        if (desired_load < 0) desired_load = 0;
        if (desired_load > 100) desired_load = 100;
    }
    else {
        while (true) {
            std::cout << "enter CPU (0-100) [default 100]: ";
            std::string line;
            if (!std::getline(std::cin, line)) {
                desired_load = 100;
                break;
            }
            line = trim(line);
            if (line.empty()) {
                desired_load = 100;
                break;
            }
            try {
                int v = std::stoi(line);
                if (v < 0 || v > 100) {
                    std::cout << "error. Required number from 0 to 100.\n";
                    continue;
                }
                desired_load = v;
                break;
            }
            catch (...) {
                std::cout << "error. Try again\n";
            }
        }
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    int cpuCount = static_cast<int>(si.dwNumberOfProcessors);

    std::cout << "Logical processors: " << cpuCount << "\n";
    std::cout << "Window size: " << WINDOW_MS << " ms\n";
    std::cout << "period from " << X0 << " to " << X1 << " step: " << DX << "\n";
    std::cout << "target load: " << desired_load << "%\n";

    std::vector<HANDLE> threads;
    threads.reserve(cpuCount);
    ThreadParam tp;
    tp.desired_load_percent = desired_load;

    auto t_start_wall = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < cpuCount; ++i) {
        HANDLE h = CreateThread(nullptr, 0, WorkerThread, &tp, 0, nullptr);
        if (!h) {
            std::cerr << "CreateThread failed: " << GetLastError() << "\n";
            timeEndPeriod(1);
            return 1;
        }
        threads.push_back(h);
    }

    WaitForMultipleObjects(static_cast<DWORD>(threads.size()), threads.data(), TRUE, INFINITE);

    auto t_end_wall = std::chrono::high_resolution_clock::now();
    double wall_ms = std::chrono::duration<double, std::milli>(t_end_wall - t_start_wall).count();

    for (HANDLE h : threads) CloseHandle(h);

    double t_cpu = 0.0;
    double t_sleep = 0.0;
    {
        std::lock_guard<std::mutex> tlock(time_mutex);
        t_cpu = total_cpu_ms;
        t_sleep = total_sleep_ms;
    }

    // Расчет достигнутой загрузки: суммарное CPU-время / (время_работы * ядра)
    double achieved_load = 0.0;
    if (wall_ms > 0 && cpuCount > 0) {
        achieved_load = 100.0 * t_cpu / (wall_ms * cpuCount);
    }

    std::cout.setf(std::ios::fixed);
    std::cout.precision(10);
    std::cout << "result: " << global_sum << "\n";
    std::cout << "wall time: " << wall_ms << " ms\n";
    std::cout << "total CPU time: " << t_cpu << " ms\n";
    std::cout << "total sleep time: " << t_sleep << " ms\n";
    std::cout << "achieved load: " << achieved_load << "% (target: " << desired_load << "%)\n";

    timeEndPeriod(1);
    return 0;
}