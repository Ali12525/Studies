#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

bool recv_line(SOCKET s, std::string& line) {
    char c;
    line.clear();
    int result;
    while (true) {
        result = recv(s, &c, 1, 0);
        if (result <= 0) {
            return false;
        }
        if (c == '\n') break;
        line.push_back(c);
    }
    return true;
}

bool send_line(SOCKET s, const std::string& line) {
    std::string data = line + "\n";
    int rc = send(s, data.c_str(), static_cast<int>(data.size()), 0);
    return rc != SOCKET_ERROR;
}

struct ThreadParam {
    long long start_idx;
    long long count;
    double x0;
    double dx;
    double sum;
};

DWORD WINAPI thread_func(LPVOID param) {
    ThreadParam* p = (ThreadParam*)param;
    double local_sum = 0.0;
    long long end_idx = p->start_idx + p->count;
    for (long long i = p->start_idx; i < end_idx; ++i) {
        double x = p->x0 + i * p->dx;
        local_sum += cos(x * x);
    }
    p->sum = local_sum;
    return 0;
}

SOCKET connect_to_server(const std::string& ip, int port) {
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    addr.sin_port = htons(static_cast<u_short>(port));
    if (connect(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(s);
        return INVALID_SOCKET;
    }
    return s;
}

// Попытка подключиться к любому серверу из списка. Возвращает сокет и обновляет список при редиректе.
SOCKET try_connect_to_any(std::vector<std::string>& server_addrs, std::string& connected_addr) {
    size_t idx = 0;
    while (idx < server_addrs.size()) {
        const auto& addr = server_addrs[idx];
        size_t colon = addr.find(':');
        if (colon == std::string::npos) { idx++; continue; }
        std::string ip = addr.substr(0, colon);
        int port = std::atoi(addr.substr(colon + 1).c_str());

        std::cout << "Connecting to " << ip << ":" << port << "...\n";
        SOCKET sock = connect_to_server(ip, port);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Failed\n";
            idx++;
            continue;
        }
        connected_addr = addr;
        return sock;
    }
    return INVALID_SOCKET;
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::vector<std::string> server_addrs;

    if (argc < 2) {
        std::cout << "Введите адреса серверов в формате ip:port через пробел (например: 127.0.0.1:12345 192.168.1.10:12345):\n";
        std::string input;
        std::getline(std::cin, input);
        std::istringstream iss(input);
        std::string addr;
        while (iss >> addr) {
            if (addr.find(':') != std::string::npos)
                server_addrs.push_back(addr);
            else
                std::cerr << "Предупреждение: пропущен некорректный адрес " << addr << "\n";
        }
        if (server_addrs.empty()) {
            std::cerr << "Не введено ни одного корректного адреса сервера.\n";
            WSACleanup();
            return 1;
        }
    }
    else {
        for (int i = 1; i < argc; ++i)
            server_addrs.push_back(argv[i]);
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET sock = INVALID_SOCKET;
    std::string current_addr;

    // Первоначальное подключение
    sock = try_connect_to_any(server_addrs, current_addr);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Could not connect to any server\n";
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to leader at " << current_addr << "\n";

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD num_threads = sysInfo.dwNumberOfProcessors;
    if (num_threads == 0) num_threads = 1;
    std::cout << "Using " << num_threads << " threads\n";

    while (true) {
        std::string line;
        if (!recv_line(sock, line)) {
            std::cerr << "Connection lost. Attempting to reconnect...\n";
            closesocket(sock);
            sock = INVALID_SOCKET;

            sock = try_connect_to_any(server_addrs, current_addr);
            if (sock == INVALID_SOCKET) {
                std::cerr << "Unable to reconnect. Exiting.\n";
                break;
            }
            std::cout << "Reconnected to leader at " << current_addr << "\n";
            continue;
        }

        if (line.empty()) continue;

        if (line == "STOP") {
            std::cout << "Received STOP\n";
            break;
        }
        if (line.rfind("TASK ", 0) == 0) {
            std::istringstream iss(line);
            std::string cmd;
            double x0, dx;
            long long nsteps;
            if (!(iss >> cmd >> x0 >> dx >> nsteps)) {
                std::cerr << "Malformed TASK\n";
                continue;
            }
            std::cout << "Received TASK: x0=" << x0 << ", nsteps=" << nsteps << "\n";

            long long total_points = nsteps + 1;
            long long base = total_points / num_threads;
            long long rem = total_points % num_threads;

            std::vector<ThreadParam> params;
            std::vector<HANDLE> threads;
            try {
                params.resize(num_threads);
                threads.resize(num_threads);
            }
            catch (const std::bad_alloc& e) {
                std::cerr << "Memory allocation failed: " << e.what() << "\n";
                break;
            }

            long long current_idx = 0;
            DWORD actual_threads = 0;
            for (DWORD i = 0; i < num_threads; ++i) {
                long long cnt = base + (i < rem ? 1 : 0);
                if (cnt == 0) {
                    threads[i] = NULL;
                    continue;
                }
                params[i].start_idx = current_idx;
                params[i].count = cnt;
                params[i].x0 = x0;
                params[i].dx = dx;
                params[i].sum = 0.0;
                threads[i] = CreateThread(NULL, 0, thread_func, &params[i], 0, NULL);
                if (threads[i] == NULL) {
                    std::cerr << "Failed to create thread " << i << "\n";
                }
                else {
                    actual_threads++;
                }
                current_idx += cnt;
            }

            if (actual_threads == 0) {
                std::cerr << "No threads created, skipping task\n";
                continue;
            }

            std::vector<HANDLE> valid_handles;
            for (auto h : threads) if (h != NULL) valid_handles.push_back(h);
            WaitForMultipleObjects(static_cast<DWORD>(valid_handles.size()), valid_handles.data(), TRUE, INFINITE);

            double total_sum = 0.0;
            for (DWORD i = 0; i < num_threads; ++i) {
                if (threads[i] != NULL) {
                    total_sum += params[i].sum;
                    CloseHandle(threads[i]);
                }
            }

            double f0 = cos(x0 * x0);
            double xn = x0 + nsteps * dx;
            double fn = cos(xn * xn);
            double integral = dx * (total_sum - 0.5 * (f0 + fn));

            std::cout << "Computed integral: " << std::setprecision(15) << integral << "\n";

            std::ostringstream oss;
            oss << "RESULT " << std::setprecision(15) << integral;
            std::string res_msg = oss.str();
            if (!send_line(sock, res_msg)) {
                std::cerr << "Failed to send result, connection may be lost, err=" << WSAGetLastError() << "\n";
            }
            else {
                std::cout << "Result sent\n";
            }
        }
        else if (line.rfind("REDIRECT ", 0) == 0) {
            std::istringstream iss(line.substr(9));
            std::string new_ip;
            int new_port;
            if (iss >> new_ip >> new_port) {
                std::cout << "Redirected to " << new_ip << ":" << new_port << "\n";
                closesocket(sock);
                sock = INVALID_SOCKET;

                // Добавляем новый адрес в начало списка
                std::ostringstream new_addr;
                new_addr << new_ip << ":" << new_port;
                server_addrs.insert(server_addrs.begin(), new_addr.str());

                // Пытаемся подключиться к новому лидеру
                sock = try_connect_to_any(server_addrs, current_addr);
                if (sock == INVALID_SOCKET) {
                    std::cerr << "Failed to connect to new leader\n";
                    break;
                }
                std::cout << "Connected to new leader at " << current_addr << "\n";
            }
        }
        else {
            std::cout << "Unknown command from server: [" << line << "]\n";
        }
    }

    if (sock != INVALID_SOCKET) closesocket(sock);
    WSACleanup();
    return 0;
}