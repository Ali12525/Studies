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
#include <thread>
#include <set>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

constexpr double X0 = 1.0;
constexpr double X1 = 2.0;
constexpr double DX = 1e-7;
const long long N_STEPS = static_cast<long long>((X1 - X0) / DX + 0.5);
constexpr int TOTAL_TIMEOUT_SEC = 3000;
constexpr int TASK_TIMEOUT_SEC = 300;
const long long CHUNK_SIZE = 100000;

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
    std::chrono::steady_clock::time_point task_start_time;
};

struct ServerInfo {
    std::string id;         // ip:server_port
    SOCKET sock;
    bool connected;
};

std::deque<Task> pending_tasks;
double global_sum = 0.0;
long long steps_done = 0;
std::vector<ServerInfo> servers;
std::string my_id;
std::string current_leader_id;
bool am_i_leader = false;
CRITICAL_SECTION cs;        

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

// Формирование идентификатора сервера
std::string make_id(const std::string& ip, int port) {
    std::ostringstream oss;
    oss << ip << ":" << port;
    return oss.str();
}

void update_leader() {
    std::set<std::string> ids;
    EnterCriticalSection(&cs);
    ids.insert(my_id);
    for (const auto& p : servers) {
        if (p.connected) ids.insert(p.id);
    }
    LeaveCriticalSection(&cs);
    if (!ids.empty()) {
        std::string new_leader = *ids.begin();
        if (new_leader != current_leader_id) {
            current_leader_id = new_leader;
            am_i_leader = (current_leader_id == my_id);
            EnterCriticalSection(&cs);
            size_t pending = pending_tasks.size();
            LeaveCriticalSection(&cs);
            std::cout << "Leader changed: " << (am_i_leader ? "I am leader" : current_leader_id)
                << " pending_tasks=" << pending
                << " steps_done=" << steps_done << "\n";
        }
    }
}

void broadcast_result(double val, long long nsteps) {
    std::ostringstream oss;
    oss << "RESULT " << std::setprecision(15) << val << " " << nsteps;
    std::string msg = oss.str();
    EnterCriticalSection(&cs);
    for (auto& p : servers) {
        if (p.connected && p.sock != INVALID_SOCKET) {
            if (!send_line(p.sock, msg)) {
                std::cerr << "Failed to send RESULT to server " << p.id << " err=" << WSAGetLastError() << "\n";
            }
        }
    }
    LeaveCriticalSection(&cs);
}

void broadcast_assign(double x0, long long nsteps) {
    std::ostringstream oss;
    oss << "ASSIGN " << std::setprecision(15) << x0 << " " << nsteps;
    std::string msg = oss.str();
    EnterCriticalSection(&cs);
    for (auto& p : servers) {
        if (p.connected && p.sock != INVALID_SOCKET) {
            if (!send_line(p.sock, msg)) {
                std::cerr << "Failed to send ASSIGN to server " << p.id << " err=" << WSAGetLastError() << "\n";
            }
        }
    }
    LeaveCriticalSection(&cs);
}

DWORD WINAPI server_listener_thread(LPVOID param) {
    SOCKET server_listen = (SOCKET)(uintptr_t)param;
    while (true) {
        sockaddr_in serverAddr;
        int addrLen = sizeof(serverAddr);
        SOCKET s = accept(server_listen, (sockaddr*)&serverAddr, &addrLen);
        if (s == INVALID_SOCKET) continue;

        std::string line;
        if (recv_line(s, line) && line.rfind("HELLO ", 0) == 0) {
            std::string their_id = line.substr(6);
            EnterCriticalSection(&cs);
            bool found = false;
            for (auto& p : servers) {
                if (p.id == their_id) {
                    p.sock = s;
                    p.connected = true;
                    found = true;
                    break;
                }
            }
            if (!found) {
                servers.push_back({ their_id, s, true });
            }
            LeaveCriticalSection(&cs);
            std::cout << "Server connected: " << their_id << "\n";
            update_leader();
        }
        else {
            closesocket(s);
        }
    }
    return 0;
}

DWORD WINAPI server_reader_thread(LPVOID param) {
    (void)param;
    while (true) {
        Sleep(100);
        EnterCriticalSection(&cs);
        for (auto& p : servers) {
            if (!p.connected || p.sock == INVALID_SOCKET) continue;
            fd_set r;
            FD_ZERO(&r);
            FD_SET(p.sock, &r);
            timeval tv = { 0, 0 };
            int rc = select(static_cast<int>(p.sock + 1), &r, nullptr, nullptr, &tv);
            if (rc > 0 && FD_ISSET(p.sock, &r)) {
                std::string line;
                if (!recv_line(p.sock, line)) {
                    std::cerr << "Server " << p.id << " disconnected\n";
                    closesocket(p.sock);
                    p.sock = INVALID_SOCKET;
                    p.connected = false;
                    continue;
                }

                if (line.rfind("RESULT ", 0) == 0) {
                    std::istringstream iss(line.substr(7));
                    double val;
                    long long nsteps;
                    if (iss >> val >> nsteps) {
                        global_sum += val;
                        steps_done += nsteps;
                        std::cout << "Received RESULT from server: " << val << ", steps_done=" << steps_done << "\n";
                    }
                }
                else if (line.rfind("ASSIGN ", 0) == 0) {
                    // удаляем соответствующую задачу из pending_tasks
                    std::istringstream iss(line.substr(7));
                    double ax0; long long an;
                    if (iss >> ax0 >> an) {
                        bool removed = false;
                        for (auto it = pending_tasks.begin(); it != pending_tasks.end(); ++it) {
                            if (it->nsteps == an && fabs(it->x0 - ax0) < 1e-12) {
                                pending_tasks.erase(it);
                                removed = true;
                                break;
                            }
                        }
                    }
                }
                else if (line.rfind("STOP", 0) == 0) {
                    std::cout << "Server requested STOP\n";
                }
            }
        }
        LeaveCriticalSection(&cs);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    if (argc < 4) {
        std::cout << "Usage: server_pool.exe <client_port> <num_clients> <server_port> [server1_ip:server_port ...]\n";
        return 1;
    }

    int client_port = std::atoi(argv[1]);
    int num_clients = std::atoi(argv[2]);
    int server_port = std::atoi(argv[3]);
    std::vector<std::string> server_args;
    for (int i = 4; i < argc; ++i) server_args.push_back(argv[i]);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    InitializeCriticalSection(&cs);

    // Определяем свой IP
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    addrinfo hints = {}, * res = nullptr;
    hints.ai_family = AF_INET;
    std::string my_ip = "127.0.0.1";
    if (getaddrinfo(hostname, nullptr, &hints, &res) == 0 && res) {
        sockaddr_in* a = (sockaddr_in*)res->ai_addr;
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &a->sin_addr, ipStr, sizeof(ipStr));
        my_ip = ipStr;
        freeaddrinfo(res);
    }
    my_id = make_id(my_ip, server_port);

    SOCKET server_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_listen == INVALID_SOCKET) {
        std::cerr << "server socket failed\n";
        WSACleanup();
        return 1;
    }
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(static_cast<u_short>(server_port));
    if (bind(server_listen, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "server bind failed\n";
        closesocket(server_listen);
        WSACleanup();
        return 1;
    }
    listen(server_listen, SOMAXCONN);

    HANDLE hServerListener = CreateThread(nullptr, 0, server_listener_thread, (LPVOID)(uintptr_t)server_listen, 0, nullptr);
    HANDLE hServerReader = CreateThread(nullptr, 0, server_reader_thread, nullptr, 0, nullptr);

    for (const auto& arg : server_args) {
        auto pos = arg.find(':');
        if (pos == std::string::npos) continue;
        std::string ip = arg.substr(0, pos);
        int port = std::atoi(arg.substr(pos + 1).c_str());
        SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s == INVALID_SOCKET) continue;
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
        addr.sin_port = htons(static_cast<u_short>(port));
        u_long mode = 1;
        ioctlsocket(s, FIONBIO, &mode);
        connect(s, (sockaddr*)&addr, sizeof(addr));
        fd_set wf;
        FD_ZERO(&wf);
        FD_SET(s, &wf);
        timeval tv = { 0, 200 };
        if (select(static_cast<int>(s + 1), nullptr, &wf, nullptr, &tv) > 0 && FD_ISSET(s, &wf)) {
            mode = 0;
            ioctlsocket(s, FIONBIO, &mode);
            if (!send_line(s, "HELLO " + my_id)) {
                closesocket(s);
                continue;
            }
            EnterCriticalSection(&cs);
            servers.push_back({ arg, s, true });
            LeaveCriticalSection(&cs);
            std::cout << "Connected to server " << arg << "\n";
        }
        else {
            closesocket(s);
        }
    }

    // Создаём сокет для клиентов
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "client socket failed\n";
        WSACleanup();
        return 1;
    }
    sockaddr_in serversAddr = {};
    serversAddr.sin_family = AF_INET;
    serversAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serversAddr.sin_port = htons(static_cast<u_short>(client_port));
    if (bind(listenSocket, (sockaddr*)&serversAddr, sizeof(serversAddr)) == SOCKET_ERROR) {
        std::cerr << "client bind failed\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    listen(listenSocket, SOMAXCONN);

    std::cout << "Server started. client_port=" << client_port << ", server_port=" << server_port << ", my_id=" << my_id << "\n";

    // Формируем очередь задач
    {
        long long steps = 0;
        while (steps < N_STEPS) {
            long long n = (std::min)(CHUNK_SIZE, N_STEPS - steps);
            double x = X0 + steps * DX;
            pending_tasks.push_back({ x, n });
            steps += n;
        }
    }

    std::vector<ClientInfo> clients;
    clients.reserve(num_clients);

    auto start_time = std::chrono::steady_clock::now();
    const auto timeout_duration = std::chrono::seconds(TOTAL_TIMEOUT_SEC);
    bool all_done = false;
    bool all_clients_connected = false;
    auto start_time_program = std::chrono::steady_clock::now();

    while (!all_done) {
        // Проверка общего таймаута
        auto now = std::chrono::steady_clock::now();
        if (now - start_time > timeout_duration) {
            std::cerr << "Global timeout expired\n";
            break;
        }

        update_leader();

        // Принимаем новых клиентов
        fd_set acceptSet;
        FD_ZERO(&acceptSet);
        FD_SET(listenSocket, &acceptSet);
        timeval tvAccept = { 0, 100 };
        if (select(static_cast<int>(listenSocket + 1), &acceptSet, nullptr, nullptr, &tvAccept) > 0) {
            sockaddr_in clientAddr;
            int addrLen = sizeof(clientAddr);
            SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
            if (clientSocket != INVALID_SOCKET) {
                if (am_i_leader) {
                    if (clients.size() < static_cast<size_t>(num_clients)) {
                        char ipStr[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, sizeof(ipStr));
                        std::cout << "Client connected: " << ipStr << "\n";
                        clients.push_back({ clientSocket, false, static_cast<int>(clients.size() + 1), false, Task(), {} });

                        if (!all_clients_connected && clients.size() >= static_cast<size_t>(num_clients)) {
                            all_clients_connected = true;
                            std::cout << "All clients connected. Starting task distribution.\n";
                        }
                    }
                    else {
                        std::cout << "Extra client rejected\n";
                        closesocket(clientSocket);
                    }
                }
                else {
                    // Мы не лидер - перенаправляем клиента
                    // Извлекаем из current_leader_id адрес и порт для клиента
                    std::string lip;
                    int lserver_port;
                    size_t pos = current_leader_id.find(':');
                    if (pos != std::string::npos) {
                        lip = current_leader_id.substr(0, pos);
                        lserver_port = std::atoi(current_leader_id.substr(pos + 1).c_str());
                        int lclient_port = lserver_port - 1; // client_port = server_port - 1
                        std::ostringstream oss;
                        oss << "REDIRECT " << lip << " " << lclient_port;
                        if (!send_line(clientSocket, oss.str())) {
                            std::cerr << "Failed to send REDIRECT to client err=" << WSAGetLastError() << "\n";
                        }
                        else {
                            std::cout << "Redirected client to " << lip << ":" << lclient_port << "\n";
                        }
                        closesocket(clientSocket);
                    }
                    else {
                        closesocket(clientSocket);
                    }
                }
            }
        }

        // Если мы лидер и все клиенты подключены - обрабатываем клиентов
        if (am_i_leader && all_clients_connected) {
            if (pending_tasks.empty() && steps_done < N_STEPS) {
                bool all_idle = true;
                for (const auto& c : clients) {
                    if (c.busy) { all_idle = false; break; }
                }
                if (all_idle) {
                    std::cerr << "WARNING: No more tasks, but steps_done (" << steps_done
                        << ") < N_STEPS (" << N_STEPS << "). Possible loss of tasks.\n";
                    std::cout << std::setprecision(15) << "Partial integral: " << global_sum << "\n";
                    all_done = true;
                }
            }

            // Выдаём задачи свободным клиентам
            for (auto& client : clients) {
                if (!client.busy && !pending_tasks.empty()) {
                    Task t;
                    EnterCriticalSection(&cs);
                    t = pending_tasks.front();
                    pending_tasks.pop_front();
                    LeaveCriticalSection(&cs);

                    std::ostringstream oss;
                    oss << "TASK " << std::setprecision(15) << t.x0 << " " << DX << " " << t.nsteps;
                    std::string task_msg = oss.str();

                    if (send_line(client.sock, task_msg)) {
                        client.busy = true;
                        client.has_task = true;
                        client.current_task = t;
                        client.task_start_time = std::chrono::steady_clock::now();
                        EnterCriticalSection(&cs);
                        size_t pending_now = pending_tasks.size();
                        LeaveCriticalSection(&cs);
                        std::cout << "Assigned task to client " << client.id << ": " << task_msg
                            << " (pending_tasks=" << pending_now << ")\n";
                        broadcast_assign(t.x0, t.nsteps);
                    }
                    else {
                        int err = WSAGetLastError();
                        std::cerr << "Failed to send task to client " << client.id << ", WSAGetLastError=" << err << "\n";
                        closesocket(client.sock);
                        client.sock = INVALID_SOCKET;
                        EnterCriticalSection(&cs);
                        pending_tasks.push_front(t);
                        LeaveCriticalSection(&cs);
                    }
                }
            }

            clients.erase(std::remove_if(clients.begin(), clients.end(),
                [](const ClientInfo& c) { return c.sock == INVALID_SOCKET; }),
                clients.end());

            // Проверяем ответы и таймауты
            fd_set readSet;
            FD_ZERO(&readSet);
            SOCKET maxFd = 0;
            for (auto& client : clients) {
                if (client.busy && client.sock != INVALID_SOCKET) {
                    FD_SET(client.sock, &readSet);
                    if (client.sock > maxFd) maxFd = client.sock;
                }
            }
            if (maxFd > 0) {
                timeval tv = { 0, 100 };
                int sel = select(static_cast<int>(maxFd + 1), &readSet, nullptr, nullptr, &tv);
                if (sel > 0) {
                    for (auto it = clients.begin(); it != clients.end();) {
                        if (it->busy && it->sock != INVALID_SOCKET && FD_ISSET(it->sock, &readSet)) {
                            std::string response;
                            if (!recv_line(it->sock, response)) {
                                std::cerr << "Client " << it->id << " disconnected\n";
                                if (it->has_task) {
                                    EnterCriticalSection(&cs);
                                    pending_tasks.push_front(it->current_task);
                                    LeaveCriticalSection(&cs);
                                }
                                closesocket(it->sock);
                                it = clients.erase(it);
                                continue;
                            }
                            std::cout << "Received from client " << it->id << ": " << response << "\n";
                            if (response.rfind("RESULT", 0) == 0) {
                                try {
                                    double val = std::stod(response.substr(7));
                                    global_sum += val;
                                    steps_done += it->current_task.nsteps;
                                    broadcast_result(val, it->current_task.nsteps);
                                    it->busy = false;
                                    it->has_task = false;
                                    ++it;
                                }
                                catch (...) {
                                    std::cerr << "Malformed RESULT\n";
                                    EnterCriticalSection(&cs);
                                    pending_tasks.push_front(it->current_task);
                                    LeaveCriticalSection(&cs);
                                    closesocket(it->sock);
                                    it = clients.erase(it);
                                    continue;
                                }
                            }
                            else {
                                std::cerr << "Unknown response\n";
                                EnterCriticalSection(&cs);
                                pending_tasks.push_front(it->current_task);
                                LeaveCriticalSection(&cs);
                                closesocket(it->sock);
                                it = clients.erase(it);
                                continue;
                            }
                        }
                        else {
                            ++it;
                        }
                    }
                }
            }

            // Проверка индивидуальных таймаутов
            auto now_check = std::chrono::steady_clock::now();
            for (auto it = clients.begin(); it != clients.end();) {
                if (it->busy) {
                    auto task_duration = now_check - it->task_start_time;
                    if (task_duration > std::chrono::seconds(TASK_TIMEOUT_SEC)) {
                        std::cerr << "Client " << it->id << " task timeout, returning task\n";
                        EnterCriticalSection(&cs);
                        pending_tasks.push_front(it->current_task);
                        LeaveCriticalSection(&cs);
                        closesocket(it->sock);
                        it = clients.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }

        // Проверка завершения (steps_done >= N_STEPS)  
        if (steps_done >= N_STEPS) {
            std::cout << std::setprecision(15) << "Final integral: " << global_sum << "\n";
            if (am_i_leader) {
                for (auto& client : clients) {
                    if (client.sock != INVALID_SOCKET) {
                        send_line(client.sock, "STOP");
                        closesocket(client.sock);
                    }
                }
                clients.clear();
                EnterCriticalSection(&cs);
                for (auto& p : servers) {
                    if (p.connected) send_line(p.sock, "STOP");
                }
                LeaveCriticalSection(&cs);
            }
            all_done = true;
        }
    }
    
    auto end = std::chrono::steady_clock::now();
    auto time_program = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time_program).count();
    std::cout << "Время выполнения программы: " << time_program << "мс";

    CloseHandle(hServerListener);
    CloseHandle(hServerReader);
    closesocket(server_listen);
    closesocket(listenSocket);
    DeleteCriticalSection(&cs);
    WSACleanup();
    return 0;
}