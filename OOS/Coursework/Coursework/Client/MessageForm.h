#pragma once
#include "Message.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include <thread>
#include <map>
#include <Windows.h>
#include <mutex>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

namespace MyClient {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace msclr::interop;
    using namespace System::Threading;

    public ref class MessageForm : public Form {
    public:
        MessageForm(SOCKET socket, String^ name) {
            InitializeComponent();
            clientSocket = socket;
            clientName = name;
            labelUserName->Text = "Пользователь: " + clientName;
            cancellationSource = gcnew CancellationTokenSource();
            listenThread = gcnew Thread(gcnew ThreadStart(this, &MessageForm::listenForMessages));
            listenThread->Start();
        }

    protected:
        ~MessageForm() {
            if (components) {
                delete components;
            }
            stopListening();
        }

    private:
        TextBox^ textBoxTo;
        TextBox^ textBoxMessage;
        Button^ buttonSend;
        TextBox^ textBoxChat;
        SOCKET clientSocket;
        String^ clientName;
        String^ incomingFrom;
        String^ incomingMessage;
        Thread^ listenThread;
        CancellationTokenSource^ cancellationSource;
        System::ComponentModel::Container^ components;
        Label^ labelTo;
        Label^ labelChat;
        Label^ labelMessage;
        Label^ labelUserName;
        Label^ labelAddContact;
        Button^ buttonSendFile;
        Button^ buttonAddContact;
        Label^ labelDelay;
        NumericUpDown^ numericUpDownDelay;
        ListBox^ listBoxContacts;
        String^ currentContact;

        void InitializeComponent(void) {
            this->components = gcnew System::ComponentModel::Container();
            this->labelTo = (gcnew Label());
            this->labelAddContact = (gcnew Label());
            this->labelChat = (gcnew Label());
            this->labelMessage = (gcnew Label());
            this->labelDelay = (gcnew Label());
            this->textBoxTo = (gcnew TextBox());
            this->textBoxMessage = (gcnew TextBox());
            this->buttonSend = (gcnew Button());
            this->buttonSendFile = (gcnew Button());
            this->buttonAddContact = (gcnew Button());
            this->textBoxChat = (gcnew TextBox());
            this->listBoxContacts = (gcnew ListBox());
            this->numericUpDownDelay = (gcnew NumericUpDown());
            this->labelUserName = (gcnew Label());

            this->SuspendLayout();

            this->labelTo->AutoSize = true;
            this->labelTo->Location = System::Drawing::Point(12, 55);
            this->labelTo->Name = L"labelTo";
            this->labelTo->Size = System::Drawing::Size(84, 13);
            this->labelTo->TabIndex = 0;
            this->labelTo->Text = L"Контакты:";
            this->labelTo->Anchor = AnchorStyles::Top | AnchorStyles::Left;

            this->labelAddContact->AutoSize = true;
            this->labelAddContact->Location = System::Drawing::Point(170, 10);
            this->labelAddContact->Name = L"labelAddContact";
            this->labelAddContact->Size = System::Drawing::Size(165, 13);
            this->labelAddContact->TabIndex = 0;
            this->labelAddContact->Text = L"Введите кого добавить в контакты:";
            this->labelAddContact->Anchor = AnchorStyles::Top | AnchorStyles::Left;

            this->textBoxTo->Location = System::Drawing::Point(170, 28);
            this->textBoxTo->Name = L"textBoxTo";
            this->textBoxTo->Size = System::Drawing::Size(590, 20);
            this->textBoxTo->TabIndex = 1;
            this->textBoxTo->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;

            this->buttonAddContact->Location = System::Drawing::Point(770, 26);
            this->buttonAddContact->Name = L"buttonAddContact";
            this->buttonAddContact->Size = System::Drawing::Size(100, 25);
            this->buttonAddContact->TabIndex = 6;
            this->buttonAddContact->Text = L"Добавить";
            this->buttonAddContact->UseVisualStyleBackColor = true;
            this->buttonAddContact->Click += gcnew System::EventHandler(this, &MessageForm::buttonAddContactClick);
            this->buttonAddContact->Anchor = AnchorStyles::Top | AnchorStyles::Right;

            this->labelChat->AutoSize = true;
            this->labelChat->Location = System::Drawing::Point(170, 54);
            this->labelChat->Name = L"labelChat";
            this->labelChat->Size = System::Drawing::Size(29, 13);
            this->labelChat->TabIndex = 2;
            this->labelChat->Text = L"Чат";
            this->labelChat->Anchor = AnchorStyles::Top | AnchorStyles::Left;

            this->textBoxChat->Location = System::Drawing::Point(170, 70);
            this->textBoxChat->Multiline = true;
            this->textBoxChat->Name = L"textBoxChat";
            this->textBoxChat->Size = System::Drawing::Size(590, 450);
            this->textBoxChat->TabIndex = 3;
            this->textBoxChat->ReadOnly = true;
            this->textBoxChat->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->textBoxChat->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom;

            this->listBoxContacts->Location = System::Drawing::Point(12, 70);
            this->listBoxContacts->Name = L"listBoxContacts";
            this->listBoxContacts->Size = System::Drawing::Size(150, 450);
            this->listBoxContacts->TabIndex = 10;
            this->listBoxContacts->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
            this->listBoxContacts->SelectedIndexChanged += gcnew System::EventHandler(this, &MessageForm::listBoxContactsSelectedIndexChanged);
            this->listBoxContacts->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Bottom;

            this->labelDelay->AutoSize = true;
            this->labelDelay->Location = System::Drawing::Point(170, 530);
            this->labelDelay->Name = L"labelDelay";
            this->labelDelay->Size = System::Drawing::Size(150, 13);
            this->labelDelay->TabIndex = 8;
            this->labelDelay->Text = L"Введите задержку в секундах";
            this->labelDelay->Anchor = AnchorStyles::Left | AnchorStyles::Bottom;

            this->numericUpDownDelay->Location = System::Drawing::Point(170, 550);
            this->numericUpDownDelay->Maximum = System::Decimal(3600);
            this->numericUpDownDelay->Name = L"numericUpDownDelay";
            this->numericUpDownDelay->Size = System::Drawing::Size(120, 20);
            this->numericUpDownDelay->TabIndex = 9;
            this->numericUpDownDelay->Anchor = AnchorStyles::Left | AnchorStyles::Bottom;

            this->labelMessage->AutoSize = true;
            this->labelMessage->Location = System::Drawing::Point(170, 580);
            this->labelMessage->Name = L"labelMessage";
            this->labelMessage->Size = System::Drawing::Size(120, 13);
            this->labelMessage->TabIndex = 4;
            this->labelMessage->Text = L"Введите сообщение";
            this->labelMessage->Anchor = AnchorStyles::Left | AnchorStyles::Bottom;

            this->textBoxMessage->Location = System::Drawing::Point(170, 600);
            this->textBoxMessage->Multiline = true;
            this->textBoxMessage->Name = L"textBoxMessage";
            this->textBoxMessage->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->textBoxMessage->Size = System::Drawing::Size(590, 60);
            this->textBoxMessage->TabIndex = 5;
            this->textBoxMessage->Anchor = AnchorStyles::Left | AnchorStyles::Right | AnchorStyles::Bottom;

            this->buttonSend->Location = System::Drawing::Point(770, 600);
            this->buttonSend->Name = L"buttonSend";
            this->buttonSend->Size = System::Drawing::Size(100, 27);
            this->buttonSend->TabIndex = 6;
            this->buttonSend->Text = L"Отправить";
            this->buttonSend->UseVisualStyleBackColor = true;
            this->buttonSend->Click += gcnew System::EventHandler(this, &MessageForm::buttonSendClick);
            this->buttonSend->Anchor = AnchorStyles::Right | AnchorStyles::Bottom;

            this->buttonSendFile->Location = System::Drawing::Point(770, 633);
            this->buttonSendFile->Name = L"buttonSendFile";
            this->buttonSendFile->Size = System::Drawing::Size(100, 27);
            this->buttonSendFile->TabIndex = 7;
            this->buttonSendFile->Text = L"Отправить файл";
            this->buttonSendFile->UseVisualStyleBackColor = true;
            this->buttonSendFile->Click += gcnew System::EventHandler(this, &MessageForm::buttonSendFileClick);
            this->buttonSendFile->Anchor = AnchorStyles::Right | AnchorStyles::Bottom;

            this->labelUserName->AutoSize = true;
            this->labelUserName->Location = System::Drawing::Point(10, 10);
            this->labelUserName->Name = L"labelUserName";
            this->labelUserName->Size = System::Drawing::Size(90, 13);
            this->labelUserName->TabIndex = 0;
            this->labelUserName->Text = L"Вы: " + clientName;
            this->labelUserName->MaximumSize = System::Drawing::Size(90, 0);
            this->labelUserName->AutoEllipsis = true;
            this->labelUserName->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;

            // MessageForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(900, 700);
            this->Controls->Add(this->listBoxContacts);
            this->Controls->Add(this->labelDelay);
            this->Controls->Add(this->numericUpDownDelay);
            this->Controls->Add(this->buttonSendFile);
            this->Controls->Add(this->labelAddContact);
            this->Controls->Add(this->buttonSend);
            this->Controls->Add(this->textBoxMessage);
            this->Controls->Add(this->textBoxChat);
            this->Controls->Add(this->buttonAddContact);
            this->Controls->Add(this->textBoxTo);
            this->Controls->Add(this->labelTo);
            this->Controls->Add(this->labelChat);
            this->Controls->Add(this->labelMessage);
            this->Controls->Add(this->labelUserName);
            this->Name = L"MessageForm";
            this->Text = L"Message";
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void buttonSendFileClick(System::Object^ sender, System::EventArgs^ e) {
            OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                String^ filePath = openFileDialog->FileName;
                sendFile(filePath);
            }
        }

        void sendFile(String^ filePath) {
            marshal_context context;
            auto selectedContacts = listBoxContacts->SelectedItems;

            if (listBoxContacts->SelectedItem != nullptr) {
                String^ to = listBoxContacts->SelectedItem->ToString();
                const char* charTo = context.marshal_as<const char*>(to);
                const char* fileName = context.marshal_as<const char*>(filePath);
                std::string fileNameStr(fileName);
                size_t lastSlash = fileNameStr.find_last_of("\\/");
                std::string shortFileName = fileNameStr.substr(lastSlash + 1);

                FILE* file;
                fopen_s(&file, fileName, "rb");
                if (file == nullptr) {
                    MessageBox::Show("Ошибка открытия файла!");
                    return;
                }

                fseek(file, 0, SEEK_END);
                long fileSize = ftell(file);
                fseek(file, 0, SEEK_SET);

                int totalPackets = (fileSize + sizeof(((StructMessage*)0)->message) - 2) / (sizeof(((StructMessage*)0)->message) - 1);
                char buffer[100];
                StructMessage msg;
                strcpy_s(msg.type, "file");
                strcpy_s(msg.from, context.marshal_as<const char*>(clientName));
                strcpy_s(msg.fileName, shortFileName.c_str());
                msg.totalPackets = totalPackets;
                msg.delaySeconds = static_cast<int>(numericUpDownDelay->Value);

                int packetNumber = 0;
                while (fileSize > 0) {
                    int bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file);
                    msg.messageSize = bytesRead;
                    memcpy_s(msg.message, sizeof(msg.message), buffer, bytesRead);
                    msg.packetNumber = packetNumber++;
                    for each(String ^ to in selectedContacts) {
                        const char* charTo = context.marshal_as<const char*>(to);
                        strcpy_s(msg.to, charTo);
                        send(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);
                    }
                    fileSize -= bytesRead;
                }

                fclose(file);
            }
            else {
                MessageBox::Show("Пожалуйста, выберите контакт для отправки сообщения!");
            }
        }

        void buttonSendClick(System::Object^ sender, System::EventArgs^ e) {
            auto selectedContacts = listBoxContacts->SelectedItems;
            if (listBoxContacts->SelectedItem != nullptr) {
                String^ to = listBoxContacts->SelectedItem->ToString();
                String^ message = textBoxMessage->Text;
                marshal_context context;
                const char* charTo = context.marshal_as<const char*>(to);
                std::string charMessage = context.marshal_as<std::string>(message);

                StructMessage msg;
                strcpy_s(msg.type, "message");
                strcpy_s(msg.from, context.marshal_as<const char*>(clientName));
                msg.delaySeconds = static_cast<int>(numericUpDownDelay->Value);
                size_t pos = 0;
                int totalPackets = (charMessage.size() + sizeof(msg.message) - 1) / sizeof(msg.message);
                msg.totalPackets = totalPackets;
                int packetNumber = 0;

                while (pos < charMessage.size()) {
                    size_t bytesToCopy = (sizeof(msg.message) < (charMessage.size() - pos)) ? sizeof(msg.message) : (charMessage.size() - pos);
                    memcpy_s(msg.message, sizeof(msg.message), charMessage.data() + pos, bytesToCopy);
                    msg.messageSize = bytesToCopy;
                    msg.packetNumber = packetNumber++;
                    for each(String ^ to in selectedContacts) {
                        const char* charTo = context.marshal_as<const char*>(to);
                        strcpy_s(msg.to, charTo);
                        send(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);
                    }
                    pos += bytesToCopy;
                }

                textBoxChat->AppendText("Вы: " + message + Environment::NewLine);
                textBoxMessage->Clear();
            }
            else {
                MessageBox::Show("Пожалуйста, выберите контакт для отправки сообщения!");
            }
        }

        String^ getSelectedContact() {
            if (listBoxContacts->InvokeRequired) {
                return (String^)listBoxContacts->Invoke(gcnew Func<String^>(this, &MessageForm::getSelectedContact));
            }
            else {
                return listBoxContacts->SelectedItem != nullptr ? listBoxContacts->SelectedItem->ToString() : nullptr;
            }
        }

        void listenForMessages() {
            StructMessage msg;
            FILE* file = nullptr;
            std::map<int, std::vector<char>> packetBuffer;
            marshal_context context;

            while (!cancellationSource->Token.IsCancellationRequested) {
                int result = recv(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);
                if (result > 0) {
                    if (strcmp(msg.type, "file") == 0) {
                        std::vector<char> packet(msg.message, msg.message + msg.messageSize);
                        packetBuffer[msg.packetNumber] = packet;

                        if (packetBuffer.size() == msg.totalPackets) {
                            System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show("Получен файл. Скачать?", "Уведомление", System::Windows::Forms::MessageBoxButtons::YesNo);
                            if (result == System::Windows::Forms::DialogResult::Yes) {
                                if (file == nullptr) {
                                    fopen_s(&file, msg.fileName, "wb");
                                    if (file == nullptr) {
                                        MessageBox::Show("Ошибка создания файла!");
                                        continue;
                                    }
                                }
                                for (const auto& packet : packetBuffer) {
                                    fwrite(packet.second.data(), 1, packet.second.size(), file);
                                }
                                fclose(file);
                                file = nullptr;
                                MessageBox::Show("Файл успешно скачан и сохранен.");
                            }
                            
                            packetBuffer.clear();
                        }
                    }
                    else if (strcmp(msg.type, "chat_history") == 0) {
                        std::vector<char> packet(msg.message, msg.message + msg.messageSize);
                        packetBuffer[msg.packetNumber] = packet;

                        if (packetBuffer.size() == msg.totalPackets) {
                            std::vector<char> completeMessage;

                            for (const auto& packet : packetBuffer) {
                                completeMessage.insert(completeMessage.end(), packet.second.begin(), packet.second.end());
                            }

                            String^ chatHistory = gcnew String(completeMessage.data(), 0, completeMessage.size());

                            if (this->IsHandleCreated) {
                                this->Invoke(gcnew Action<String^>(this, &MessageForm::appendChatHistory), chatHistory);
                            }

                            packetBuffer.clear();
                        }
                    }
                    else if (strcmp(msg.type, "update_contacts") == 0) {
                        String^ contactsList = gcnew String(msg.message);
                        if (this->IsHandleCreated) {
                            this->Invoke(gcnew Action<String^>(this, &MessageForm::updateContactsList), contactsList);
                        }
                    }
                    else {
                        std::vector<char> packet(msg.message, msg.message + msg.messageSize);
                        packetBuffer[msg.packetNumber] = packet;

                        if (packetBuffer.size() == msg.totalPackets) {
                            String^ selectedContact = getSelectedContact();
                            marshal_context context;
                            if (selectedContact != nullptr) {
                                std::string contactStr = context.marshal_as<std::string>(selectedContact);
                                if (contactStr == msg.from) {
                                    std::vector<char> completeMessage;
                                    for (const auto& packet : packetBuffer) {
                                        completeMessage.insert(completeMessage.end(), packet.second.begin(), packet.second.end());
                                    }
                                    incomingFrom = gcnew String(msg.from);
                                    incomingMessage = gcnew String(completeMessage.data(), 0, completeMessage.size());
                                    if (this->IsHandleCreated) {
                                        this->Invoke(gcnew MethodInvoker(this, &MessageForm::updateChat));
                                    }
                                }
                            }

                            saveMessageToServer(packetBuffer, msg.from, msg.to);
                            saveMessageToServer(packetBuffer, msg.to, msg.from);

                            packetBuffer.clear();
                        }
                    }
                }
            }
        }

        void saveMessageToServer(const std::map<int, std::vector<char>>& packetBuffer, const char* from, const char* to) {
            StructMessage msgTo;
            strcpy_s(msgTo.type, "full_chat");
            strcpy_s(msgTo.from, to);
            strcpy_s(msgTo.to, from);
            msgTo.delaySeconds = 0;
            marshal_context context;
            std::string combinedString;
            
            if (context.marshal_as<std::string>(clientName) == to) {
                combinedString = std::string(msgTo.to) + ": ";
            }
            else {
                combinedString = "Вы: ";
            }
            
            strcpy_s(msgTo.message, combinedString.c_str());
            msgTo.messageSize = combinedString.size();
            send(clientSocket, reinterpret_cast<char*>(&msgTo), sizeof(msgTo), 0);

            StructMessage msg;
            strcpy_s(msg.type, "full_chat");
            strcpy_s(msg.from, to);
            strcpy_s(msg.to, from);
            msg.delaySeconds = 0;
            msg.totalPackets = packetBuffer.size();
            int packetNumber = 0;

            for (const auto& packet : packetBuffer) {
                memcpy_s(msg.message, sizeof(msg.message), packet.second.data(), packet.second.size());
                msg.messageSize = packet.second.size();
                msg.packetNumber = packetNumber++;
                send(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);
            }

            StructMessage msgToN;
            strcpy_s(msgToN.type, "full_chat");
            strcpy_s(msgToN.from, to);
            strcpy_s(msgToN.to, from);
            msgToN.delaySeconds = 0;
            msgToN.message[0] = '\n';
            msgToN.messageSize = 1;
            send(clientSocket, reinterpret_cast<char*>(&msgToN), sizeof(msgToN), 0);
        }

        void appendChatHistory(String^ chatHistory) {
            if (this->IsHandleCreated) {
                textBoxChat->Clear();
                array<String^>^ lines = chatHistory->Split('\n');
                for each(String ^ line in lines) {
                    textBoxChat->AppendText(line + Environment::NewLine);
                }
            }
        }

        void updateChat() {
            textBoxChat->AppendText(incomingFrom + ": " + incomingMessage + Environment::NewLine);
        }

        void updateContactsList(String^ contactsList) {
            listBoxContacts->Items->Clear();
            array<String^>^ contacts = contactsList->Split(',');
            for each(String ^ contact in contacts) {
                listBoxContacts->Items->Add(contact);
            }
        }

        void listBoxContactsSelectedIndexChanged(Object^ sender, EventArgs^ e) {
            if (listBoxContacts->SelectedItem != nullptr) {
                currentContact = listBoxContacts->SelectedItem->ToString();
                textBoxChat->Clear();
                loadChatHistory(currentContact);
            }
        }

        void loadChatHistory(String^ contact) {
            StructMessage msg;
            strcpy_s(msg.type, "load_chat");
            marshal_context context;
            strcpy_s(msg.from, context.marshal_as<const char*>(clientName));
            strcpy_s(msg.to, context.marshal_as<const char*>(contact));
            send(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);
        }

        void stopListening() {
            cancellationSource->Cancel();
            closesocket(clientSocket);
            if (listenThread != nullptr) {
                listenThread->Join();
            }
        }

        void addContact(String^ contactName) {
            marshal_context context;
            const char* charContactName = context.marshal_as<const char*>(contactName);

            size_t maxSizeOfTo = sizeof(StructMessage::to);
            if (strlen(charContactName) >= maxSizeOfTo) {
                MessageBox::Show("Имя слишком длинное!");
                return;
            }
            textBoxTo->Clear();

            StructMessage msg;
            strcpy_s(msg.type, "add_contact");
            strcpy_s(msg.from, context.marshal_as<const char*>(clientName));
            strncpy_s(msg.to, charContactName, maxSizeOfTo - 1);
            msg.to[maxSizeOfTo - 1] = '\0';
            send(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);
        }

        void buttonAddContactClick(Object^ sender, EventArgs^ e) {
            addContact(textBoxTo->Text);
        }

        void notifyServerAboutExit() {
            StructMessage msg;
            strcpy_s(msg.type, "disconnect");
            marshal_context context;
            const char* charName = context.marshal_as<const char*>(clientName);
            strncpy_s(msg.from, charName, sizeof(msg.from) - 1);
            msg.from[sizeof(msg.from) - 1] = '\0';


            send(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);
        }

    protected:
        void OnFormClosing(System::Windows::Forms::FormClosingEventArgs^ e) override {
            notifyServerAboutExit();
            stopListening();
            Form::OnFormClosing(e);
        }
    };
}
