#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

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

    std::string server_ip;
    int port;

    if (argc >= 3) {
        server_ip = argv[1];
        port = std::atoi(argv[2]);
    }
    else {
        std::cout << "Введите IP сервера: ";
        std::cin >> server_ip;
        std::cout << "Введите порт: ";
        std::cin >> port;
    }

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, server_ip.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(static_cast<u_short>(port));

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Подключено к серверу " << server_ip << ":" << port << "\n";

    while (true) {
        std::string task_line;
        if (!recv_line(clientSocket, task_line)) {
            std::cerr << "Сервер закрыл соединение или ошибка чтения.\n";
            break;
        }

        if (task_line.empty()) {
            std::cout << "Получена пустая строка, завершаем работу.\n";
            break;
        }

        std::cout << "Получено задание: " << task_line << "\n";

        std::istringstream iss(task_line);
        std::string cmd;
        double x0, dx;
        long long nsteps;

        if (!(iss >> cmd >> x0 >> dx >> nsteps) || cmd != "TASK") {
            std::cerr << "Неверный формат задания: " << task_line << "\n";
            break;
        }

        auto start_time = std::chrono::steady_clock::now();
        // Вычисление интеграла
        double local_sum = 0.0;
        for (long long i = 0; i <= nsteps; ++i) {
            double x = x0 + i * dx;
            double fx = std::cos(x * x);
            double weight = (i == 0 || i == nsteps) ? 0.5 : 1.0;
            local_sum += weight * fx;
        }
        local_sum *= dx;

        auto end = std::chrono::steady_clock::now();
        auto time_program = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count();

        std::cout << "Результат вычислений: " << std::setprecision(15) << local_sum << "\n";

        std::ostringstream oss;
        oss << "RESULT " << std::setprecision(15) << local_sum;
        if (!send_line(clientSocket, oss.str())) {
            std::cerr << "Ошибка отправки результата серверу\n";
            break;
        }
        std::cout << "Результат отправлен серверу\n";
        std::cout << "Время выполнения: " << time_program << "мс";

    }

    getchar();
    getchar();

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}