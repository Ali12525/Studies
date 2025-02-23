#include <iostream>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include <random>
#include <cstdio>
#include <string>
#include <locale.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Message {
    char type[100];
    char from[100];
    char to[100];
    char message[100];
    char filename[100];
    int delaySeconds;
    int packetNumber;
    int totalPackets;
    size_t messageSize;
};

struct User {
    SOCKET socket;
    string password;
    vector<string> contacts;
};

unordered_map<string, User> clients;
mutex fileMutex;

void saveClients() {
    ofstream out("clients.txt");
    for (const auto& pair : clients) {
        out << pair.first << " " << pair.second.password;
        for (const auto& contact : pair.second.contacts) {
            out << " " << contact;
        }
        out << endl;
    }
    out.close();
}

void loadClients() {
    ifstream in("clients.txt");
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string name, password, contact;
        ss >> name >> password;
        clients[name] = { INVALID_SOCKET, password };
        while (ss >> contact) {
            clients[name].contacts.push_back(contact);
        }
    }
    in.close();
}

void saveChatHistory(const string& from, const string& to, const char* message, size_t messageSize) {
    lock_guard<mutex> lock(fileMutex);
    string filename = from + "_" + to + "_chat.txt";
    ofstream chatLog(filename, ios_base::app);
    chatLog.write(message, messageSize);
}

string loadChatHistory(const string& from, const string& to) {
    lock_guard<mutex> lock(fileMutex);
    string filename = from + "_" + to + "_chat.txt";
    ifstream chatLog(filename);
    stringstream buffer;
    buffer << chatLog.rdbuf();
    return buffer.str();
}

void sendMessageToClients(const Message& msg) {
    if (clients.find(msg.to) != clients.end() && clients[msg.to].socket != INVALID_SOCKET) {
        int result = send(clients[msg.to].socket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
        if (result == SOCKET_ERROR) {
            cerr << "Не удалось отправить сообщение на " << msg.to << ". Ошибка: " << WSAGetLastError() << endl;
        }
    }
}

void sendDataMessageWithDelay(const Message& originalMsg) {
    this_thread::sleep_for(chrono::seconds(originalMsg.delaySeconds));
    sendMessageToClients(originalMsg);
}

void handleClient(SOCKET clientSocket) {
    Message msg;
    while (recv(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0) > 0) {
        if (strcmp(msg.type, "register") == 0) {
            if (clients.find(msg.from) == clients.end()) {
                clients[msg.from] = { clientSocket, msg.message };
                saveClients();
                cout << "Пользователь " << msg.from << " зарегистрировался." << endl;
                strcpy_s(msg.type, "response");
                strcpy_s(msg.message, "Registration successful");
                send(clientSocket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
            }
            else {
                cerr << "Регистрация не удалась. Пользователь " << msg.from << " уже существует." << endl;
                strcpy_s(msg.type, "response");
                strcpy_s(msg.message, "Такой пользователь уже есть!");
                send(clientSocket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
            }
        }
        else if (strcmp(msg.type, "login") == 0) {
            if (clients.find(msg.from) != clients.end() && clients[msg.from].password == msg.message) {
                clients[msg.from].socket = clientSocket;
                cout << "Пользователь " << msg.from << " вошел в систему." << endl;
                strcpy_s(msg.type, "response");
                strcpy_s(msg.message, "Login successful");
                send(clientSocket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);

                stringstream ss;
                for (const auto& contact : clients[msg.from].contacts) {
                    ss << contact << ",";
                }
                string contactsList = ss.str();
                if (!contactsList.empty()) {
                    contactsList.pop_back();
                }
                strcpy_s(msg.type, "update_contacts");
                strncpy_s(msg.message, contactsList.c_str(), sizeof(msg.message) - 1);
                send(clientSocket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
            }
            else if (clients.find(msg.from) == clients.end()) {
                cerr << "Не удалось войти в систему. Пользователь " << msg.from << " не зарегистрирован." << endl;
                strcpy_s(msg.type, "response");
                strcpy_s(msg.message, "Пользователь не зарегестрирован!");
                send(clientSocket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
            }
            else {
                cerr << "Не удалось войти в систему для " << msg.from << ". Неверный пароль." << endl;
                strcpy_s(msg.type, "response");
                strcpy_s(msg.message, "Не верный пароль!");
                send(clientSocket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
            }
        }
        else if (strcmp(msg.type, "message") == 0 || strcmp(msg.type, "file") == 0) {
            thread(sendDataMessageWithDelay, msg).detach();
        }
        else if (strcmp(msg.type, "full_chat") == 0) {
            saveChatHistory(msg.from, msg.to, msg.message, msg.messageSize);
        }
        else if (strcmp(msg.type, "add_contact") == 0) {
            string contactName(msg.to);
            if (clients.find(contactName) != clients.end()) {
                if (find(clients[msg.from].contacts.begin(), clients[msg.from].contacts.end(), contactName) == clients[msg.from].contacts.end()) {
                    clients[msg.from].contacts.push_back(contactName);
                    saveClients();
                    stringstream ss;
                    for (const auto& contact : clients[msg.from].contacts) {
                        ss << contact << ",";
                    }
                    string contactsList = ss.str();
                    if (!contactsList.empty()) {
                        contactsList.pop_back();
                    }
                    strcpy_s(msg.type, "update_contacts");
                    strncpy_s(msg.message, contactsList.c_str(), sizeof(msg.message) - 1);
                    send(clientSocket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
                }
                else {
                    cerr << "Контакт " << contactName << " уже существует для пользователя " << msg.from << "." << endl;
                    
                }
            }
            else {
                cerr << "Пользователь " << contactName << " не найден." << endl;
                
            }
        }
        else if (strcmp(msg.type, "load_chat") == 0) {
            string chatHistory = loadChatHistory(msg.from, msg.to);
            Message chatMsg;
            strcpy_s(chatMsg.type, sizeof(chatMsg.type), "chat_history");
            strcpy_s(chatMsg.from, sizeof(chatMsg.from), "server");
            strcpy_s(chatMsg.to, sizeof(chatMsg.to), msg.from);

            size_t pos = 0;
            
            int totalPackets = (chatHistory.size() + sizeof(chatMsg.message) - 1) / sizeof(chatMsg.message);
            chatMsg.totalPackets = totalPackets;
            int packetNumber = 0;

            while (pos < chatHistory.size()) {
                size_t bytesToCopy = (sizeof(chatMsg.message) < (chatHistory.size() - pos)) ? sizeof(chatMsg.message) : (chatHistory.size() - pos);
                memcpy_s(chatMsg.message, sizeof(chatMsg.message), chatHistory.data() + pos, bytesToCopy);
                chatMsg.messageSize = bytesToCopy;
                chatMsg.packetNumber = packetNumber++;

                send(clientSocket, reinterpret_cast<char*>(&chatMsg), sizeof(chatMsg), 0);

                pos += bytesToCopy;
            }
        }
        else if (strcmp(msg.type, "disconnect") == 0) {
            cerr << "Клиент " << msg.from << " отключился." << endl;
        }
    }
    closesocket(clientSocket);
}

int main() {
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    loadClients();

    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    cout << "Сервер запущен. Ожидание клиентов..." << endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        thread(handleClient, clientSocket).detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
