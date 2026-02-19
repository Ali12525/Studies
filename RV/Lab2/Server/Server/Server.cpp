#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <deque>
#include <chrono>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

constexpr double X0 = 1.0;
constexpr double X1 = 2.0;
constexpr double DX = 1e-7;
const long long N_STEPS = static_cast<long long>((X1 - X0) / DX + 0.5);
constexpr int TOTAL_TIMEOUT_SEC = 300;

struct Task {
    double x0;
    long long nsteps;
};

struct ClientInfo {
    SOCKET sock;
    bool busy;
    int id;
    bool has_task;
    Task current_task;
};

bool recv_line(SOCKET s, std::string& line) {
    char c;
    line.clear();
    int result;
    while ((result = recv(s, &c, 1, 0)) > 0) {
        if (c == '\n') break;
        line.push_back(c);
    }
    return result > 0 || !line.empty();
}

bool send_line(SOCKET s, const std::string& line) {
    std::string data = line + "\n";
    return send(s, data.c_str(), static_cast<int>(data.size()), 0) != SOCKET_ERROR;
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int port = 12345;
    int num_clients = 1;

    if (argc >= 3) {
        port = std::atoi(argv[1]);
        num_clients = std::atoi(argv[2]);
        if (num_clients <= 0) num_clients = 1;
    }
    else {
        std::cout << "Введите порт: ";
        std::cin >> port;
        std::cout << "Введите количество клиентов: ";
        std::cin >> num_clients;
        if (num_clients <= 0) num_clients = 1;
    }

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(static_cast<u_short>(port));

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Сервер запущен на порту " << port << ", ожидаем " << num_clients << " клиентов...\n";

    std::vector<ClientInfo> clients;
    clients.reserve(num_clients);

    while (clients.size() < static_cast<size_t>(num_clients)) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, sizeof(ipStr));
        std::cout << "Подключился клиент " << clients.size() + 1 << ": " << ipStr << "\n";
        clients.push_back({ clientSocket, false, static_cast<int>(clients.size() + 1), false, Task() });
    }

    std::cout << "Все клиенты подключены. Раздаём задания...\n";

    long long totalSteps = N_STEPS;
    long long stepsPerClient = totalSteps / num_clients;
    long long stepsRem = totalSteps % num_clients;

    std::deque<Task> pending_tasks;

    for (int i = 0; i < num_clients; ++i) {
        long long startStep = i * stepsPerClient + (i < stepsRem ? i : stepsRem);
        long long nsteps = stepsPerClient + (i < stepsRem ? 1 : 0);
        double x0 = X0 + startStep * DX;
        pending_tasks.push_back({ x0, nsteps });
    }

    clients.erase(std::remove_if(clients.begin(), clients.end(),
        [&](ClientInfo& client) {
            if (pending_tasks.empty()) return false;
            Task t = pending_tasks.front();
            pending_tasks.pop_front();
            std::ostringstream oss;
            oss << "TASK " << std::setprecision(15) << t.x0 << " " << DX << " " << t.nsteps;
            if (send_line(client.sock, oss.str())) {
                client.busy = true;
                client.has_task = true;
                client.current_task = t;
                std::cout << "Задание клиенту " << client.id << ": " << oss.str() << "\n";
                return false;
            }
            else {
                std::cerr << "Ошибка отправки задания клиенту " << client.id << ", клиент удаляется.\n";
                closesocket(client.sock);
                pending_tasks.push_front(t);
                return true;
            }
        }), clients.end());

    double global_sum = 0.0;
    auto start_time = std::chrono::steady_clock::now();
    const auto timeout_duration = std::chrono::seconds(TOTAL_TIMEOUT_SEC);

    while (!pending_tasks.empty() || std::any_of(clients.begin(), clients.end(), [](const ClientInfo& c) { return c.busy; })) {
        auto now = std::chrono::steady_clock::now();
        if (now - start_time > timeout_duration) {
            std::cerr << "Ошибка: истек общий таймаут (" << TOTAL_TIMEOUT_SEC << " с).\n";
            break;
        }

        fd_set readSet;
        FD_ZERO(&readSet);
        SOCKET maxFd = 0;
        for (auto& client : clients) {
            if (client.busy) {
                FD_SET(client.sock, &readSet);
                if (client.sock > maxFd) maxFd = client.sock;
            }
        }

        if (maxFd == 0 && !pending_tasks.empty()) {
            for (auto& client : clients) {
                if (!client.busy && !pending_tasks.empty()) {
                    Task t = pending_tasks.front();
                    pending_tasks.pop_front();
                    std::ostringstream oss;
                    oss << "TASK " << std::setprecision(15) << t.x0 << " " << DX << " " << t.nsteps;
                    if (send_line(client.sock, oss.str())) {
                        client.busy = true;
                        client.has_task = true;
                        client.current_task = t;
                        std::cout << "Доп. задание клиенту " << client.id << ": " << oss.str() << "\n";
                    }
                    else {
                        std::cerr << "Ошибка отправки доп. задания клиенту " << client.id << ", клиент удаляется.\n";
                        closesocket(client.sock);
                        pending_tasks.push_front(t);
                        auto it = std::find_if(clients.begin(), clients.end(), [&client](const ClientInfo& c) { return c.sock == client.sock; });
                        if (it != clients.end()) clients.erase(it);
                        break;
                    }
                }
            }
            continue;
        }

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
        long timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeout_duration - elapsed).count();
        if (timeout_ms < 0) timeout_ms = 0;

        timeval tv;
        tv.tv_sec = static_cast<long>(timeout_ms / 1000);
        tv.tv_usec = static_cast<long>((timeout_ms % 1000) * 1000);

        int selectResult = select(static_cast<int>(maxFd + 1), &readSet, nullptr, nullptr, &tv);
        if (selectResult == SOCKET_ERROR) {
            std::cerr << "select failed: " << WSAGetLastError() << std::endl;
            break;
        }
        if (selectResult == 0) {
            for (auto it = clients.begin(); it != clients.end(); ) {
                if (it->busy) {
                    std::cerr << "Клиент " << it->id << " не отвечает, удаляем.\n";
                    if (it->has_task) {
                        pending_tasks.push_front(it->current_task);
                    }
                    closesocket(it->sock);
                    it = clients.erase(it);
                }
                else {
                    ++it;
                }
            }
            continue;
        }

        for (auto it = clients.begin(); it != clients.end(); ) {
            if (it->busy && FD_ISSET(it->sock, &readSet)) {
                std::string response;
                if (!recv_line(it->sock, response)) {
                    std::cerr << "Клиент " << it->id << " отключился или ошибка чтения.\n";
                    if (it->has_task) {
                        pending_tasks.push_front(it->current_task);
                    }
                    closesocket(it->sock);
                    it = clients.erase(it);
                    continue;
                }
                std::cout << "Получен результат от клиента " << it->id << ": " << response << "\n";
                if (response.find("RESULT") == 0) {
                    double val = std::stod(response.substr(7));
                    global_sum += val;
                    it->busy = false;
                    it->has_task = false;

                    if (!pending_tasks.empty()) {
                        Task t = pending_tasks.front();
                        pending_tasks.pop_front();
                        std::ostringstream oss;
                        oss << "TASK " << std::setprecision(15) << t.x0 << " " << DX << " " << t.nsteps;
                        if (send_line(it->sock, oss.str())) {
                            it->busy = true;
                            it->has_task = true;
                            it->current_task = t;
                            std::cout << "Доп. задание клиенту " << it->id << ": " << oss.str() << "\n";
                        }
                        else {
                            std::cerr << "Ошибка отправки доп. задания клиенту " << it->id << ", клиент удаляется.\n";
                            pending_tasks.push_front(t);
                            closesocket(it->sock);
                            it = clients.erase(it);
                            continue;
                        }
                    }
                }
                else {
                    std::cerr << "Неизвестный формат ответа: " << response << "\n";
                    if (it->has_task) {
                        pending_tasks.push_front(it->current_task);
                    }
                    closesocket(it->sock);
                    it = clients.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto time_program = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count();

    if (!pending_tasks.empty()) {
        std::cerr << "Остались невыполненные задания. Результат может быть неполным.\n";
    }
    std::cout << std::setprecision(15) << "Итоговое значение интеграла: " << global_sum << "\n";
    std::cout << "Время выполнения программы: " << time_program << "мс";

    for (auto& client : clients) closesocket(client.sock);
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}