#pragma once
#include "MessageForm.h"
#include "Message.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

#pragma comment(lib, "ws2_32.lib")

namespace MyClient {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace msclr::interop;

    public ref class RegisterForm : public System::Windows::Forms::Form {
    public:
        RegisterForm(void) {
            InitializeComponent();
            InitializeSocket();
        }

    protected:
        ~RegisterForm() {
            if (components) {
                delete components;
            }
        }
    private:
        System::Windows::Forms::Label^ labelName;
        System::Windows::Forms::Label^ labelPassword;
        System::Windows::Forms::TextBox^ textBoxName;
        System::Windows::Forms::TextBox^ textBoxPassword;
        System::Windows::Forms::Button^ buttonRegister;
        System::Windows::Forms::Button^ buttonLogin;
        System::Windows::Forms::CheckBox^ checkBoxShowPassword;
        SOCKET clientSocket;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void) {
            this->labelName = (gcnew System::Windows::Forms::Label());
            this->labelPassword = (gcnew System::Windows::Forms::Label());
            this->textBoxName = (gcnew System::Windows::Forms::TextBox());
            this->textBoxPassword = (gcnew System::Windows::Forms::TextBox());
            this->buttonRegister = (gcnew System::Windows::Forms::Button());
            this->buttonLogin = (gcnew System::Windows::Forms::Button());
            this->checkBoxShowPassword = (gcnew System::Windows::Forms::CheckBox());
            this->SuspendLayout();
            
            this->labelName->AutoSize = true;
            this->labelName->Font = (gcnew System::Drawing::Font(L"Arial", 12));
            this->labelName->Location = System::Drawing::Point(15, 16);
            this->labelName->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->labelName->Name = L"labelName";
            this->labelName->Size = System::Drawing::Size(106, 18);
            this->labelName->TabIndex = 0;
            this->labelName->Text = L"Введите имя:";
            
            this->labelPassword->AutoSize = true;
            this->labelPassword->Font = (gcnew System::Drawing::Font(L"Arial", 12));
            this->labelPassword->Location = System::Drawing::Point(15, 86);
            this->labelPassword->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
            this->labelPassword->Name = L"labelPassword";
            this->labelPassword->Size = System::Drawing::Size(130, 18);
            this->labelPassword->TabIndex = 2;
            this->labelPassword->Text = L"Введите пароль:";
            
            this->textBoxName->Font = (gcnew System::Drawing::Font(L"Arial", 12));
            this->textBoxName->Location = System::Drawing::Point(15, 41);
            this->textBoxName->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->textBoxName->Name = L"textBoxName";
            this->textBoxName->Size = System::Drawing::Size(289, 26);
            this->textBoxName->TabIndex = 1;
            
            this->textBoxPassword->Font = (gcnew System::Drawing::Font(L"Arial", 12));
            this->textBoxPassword->Location = System::Drawing::Point(15, 111);
            this->textBoxPassword->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->textBoxPassword->Name = L"textBoxPassword";
            this->textBoxPassword->PasswordChar = '*';
            this->textBoxPassword->Size = System::Drawing::Size(289, 26);
            this->textBoxPassword->TabIndex = 3;
            
            this->buttonRegister->Font = (gcnew System::Drawing::Font(L"Arial", 12));
            this->buttonRegister->Location = System::Drawing::Point(15, 193);
            this->buttonRegister->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->buttonRegister->Name = L"buttonRegister";
            this->buttonRegister->Size = System::Drawing::Size(130, 32);
            this->buttonRegister->TabIndex = 5;
            this->buttonRegister->Text = L"Регистрация";
            this->buttonRegister->UseVisualStyleBackColor = true;
            this->buttonRegister->Click += gcnew System::EventHandler(this, &RegisterForm::buttonRegisterClick);
             
            this->buttonLogin->Font = (gcnew System::Drawing::Font(L"Arial", 12));
            this->buttonLogin->Location = System::Drawing::Point(174, 193);
            this->buttonLogin->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->buttonLogin->Name = L"buttonLogin";
            this->buttonLogin->Size = System::Drawing::Size(130, 32);
            this->buttonLogin->TabIndex = 6;
            this->buttonLogin->Text = L"Вход";
            this->buttonLogin->UseVisualStyleBackColor = true;
            this->buttonLogin->Click += gcnew System::EventHandler(this, &RegisterForm::buttonLoginClick);
            
            this->checkBoxShowPassword->AutoSize = true;
            this->checkBoxShowPassword->Font = (gcnew System::Drawing::Font(L"Arial", 10));
            this->checkBoxShowPassword->Location = System::Drawing::Point(15, 150);
            this->checkBoxShowPassword->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->checkBoxShowPassword->Name = L"checkBoxShowPassword";
            this->checkBoxShowPassword->Size = System::Drawing::Size(138, 20);
            this->checkBoxShowPassword->TabIndex = 4;
            this->checkBoxShowPassword->Text = L"Показать пароль";
            this->checkBoxShowPassword->UseVisualStyleBackColor = true;
            this->checkBoxShowPassword->CheckedChanged += gcnew System::EventHandler(this, &RegisterForm::checkBoxShowPasswordCheckedChanged);
            
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(326, 249);
            this->Controls->Add(this->buttonLogin);
            this->Controls->Add(this->buttonRegister);
            this->Controls->Add(this->checkBoxShowPassword);
            this->Controls->Add(this->textBoxPassword);
            this->Controls->Add(this->labelPassword);
            this->Controls->Add(this->textBoxName);
            this->Controls->Add(this->labelName);
            this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
            this->Name = L"RegisterForm";
            this->Text = L"Вход/Регистрация";
            this->ResumeLayout(false);
            this->PerformLayout();

        }

        void InitializeSocket() {
            WSAData wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);
            clientSocket = socket(AF_INET, SOCK_STREAM, 0);

            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            server_addr.sin_port = htons(8080);

            connect(clientSocket, (sockaddr*)&server_addr, sizeof(server_addr));
        }

        void buttonRegisterClick(System::Object^ sender, System::EventArgs^ e) {
            registerOrLogin("register");
        }

        void buttonLoginClick(System::Object^ sender, System::EventArgs^ e) {
            registerOrLogin("login");
        }

        void checkBoxShowPasswordCheckedChanged(System::Object^ sender, System::EventArgs^ e) {
            if (checkBoxShowPassword->Checked) {
                textBoxPassword->PasswordChar = '\0';
            }
            else {
                textBoxPassword->PasswordChar = '*';
            }
        }

        void registerOrLogin(const char* type) {
            String^ name = textBoxName->Text;
            String^ password = textBoxPassword->Text;
            marshal_context context;
            const char* charName = context.marshal_as<const char*>(name);
            const char* charPassword = context.marshal_as<const char*>(password);

            size_t maxSizeOfFrom = sizeof(StructMessage::from);
            size_t maxSizeOfMessage = sizeof(StructMessage::message);

            if (strlen(charName) >= maxSizeOfFrom) {
                MessageBox::Show("Имя слишком длинное!");
                return;
            } 
            if (strlen(charPassword) >= maxSizeOfMessage) {
                MessageBox::Show("Пароль слишком длинный!");
                return;
            }

            StructMessage msg;
            strcpy(msg.type, type);
            strncpy_s(msg.from, charName, maxSizeOfFrom - 1);
            msg.from[maxSizeOfFrom - 1] = '\0';
            strcpy(msg.to, "");
            strncpy_s(msg.message, charPassword, maxSizeOfMessage - 1);
            msg.message[maxSizeOfMessage - 1] = '\0';

            send(clientSocket, reinterpret_cast<char*>(&msg), sizeof(msg), 0);

            // Ждем ответа от сервера
            StructMessage response;
            recv(clientSocket, reinterpret_cast<char*>(&response), sizeof(response), 0);

            // Обработка ответа
            String^ serverMessage = gcnew String(response.message);

            if (serverMessage == "Registration successful") {
                MessageBox::Show("Регистрация прошла успешно!");
            }
            else if (serverMessage == "Login successful") {
                MessageBox::Show("Вход выполнен успешно!");
                this->Hide();
                MessageForm^ messageForm = gcnew MessageForm(clientSocket, name);
                messageForm->ShowDialog();
                this->Close();
            }
            else {
                MessageBox::Show(serverMessage);
            }

        }
    };
}