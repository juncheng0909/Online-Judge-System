#include <iostream>
#include <fstream>
#include <utility>
#include <sstream>
#include "AccountSystem.h"
#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

User* AccountSystem::search(std::string name) {
    for(size_t i=0;i<user_list.size();i++) {
        if(user_list[i].getUsername() == name){
            return &user_list[i];
        }
    }
    return nullptr;
}

void AccountSystem::init(std::string USER_DATA_PATH) {

    AccountSystem::USER_DATA_PATH = USER_DATA_PATH;

    try {

        std::ifstream file(USER_DATA_PATH); // 開啟檔案
        if( !file ) {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        // TODO: Loading user data from USER_DATA_PATH and call adding_user (from AccountSystem) function to insert data
        // Hints: stringstream

        std::string read_line;
        while( getline(file, read_line) ) {
            std::string username, password;
            std::stringstream ss(read_line);
            std::getline(ss, username, ',');
            std::getline(ss, password, ',');
            AccountSystem::adding_user(username, password);
        }
        file.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
}

void AccountSystem::sign_up() {
    std::string username, password, password_confirm;

    std::cout << yellow("Welcome! please enter your name: ");
    std::getline(std::cin, username);

    while (true) {
        std::cout << yellow("PLease enter your password: ");
        std::getline(std::cin, password);

        std::cout << yellow("PLease enter your password again: ");
        std::getline(std::cin, password_confirm);

        if (password == password_confirm) {
            break;  // 密碼確認成功，跳出迴圈
        } else {
            std::cout << red("Passwords do not match. Please try again.\n");
        }
    }

    adding_user(username, password);

    return;
}

std::pair<bool, std::string> AccountSystem::login() {
    std::pair<bool, std::string> result = std::make_pair(false, "");

    while (true) {
        std::string username;
        std::cout << yellow("User name(Enter -1 to sign up): ");
        std::getline(std::cin, username);

        if (username == "-1") {
            sign_up();
            return result;  // 回傳 (false, "")
        }

        User* user_ptr = search(username);
        if (user_ptr == nullptr) {
            std::cout << red("User is not exist!\n");
            continue;
        }

        int retry = 3;
        while (retry--) {
            std::string password;
            std::cout << white("Welcome aboard, ")<<username<<".\n";
            std::cout << white("Please enter your password: ");
            std::getline(std::cin, password);

            if (password == user_ptr->getPassword()) {
                std::cout << green("Login Success!!! Welcome aboard\n");
                login_user = username;  // 記錄登入者
                return std::make_pair(true, username);
            } else {
                std::cout << red("Password incorrect... please try again: \n");
                continue;
            }
        }

        std::cout << red("To many unsuccessful sign-in attempts... \n");
    }

    return result;
}

void AccountSystem::adding_user(std::string username, std::string password) {
    User new_user(username, password);
    user_list.push_back(new_user);
    AccountSystem::userdataUpdate();
    return;
}
std::string AccountSystem::getuserLogin() {
    return AccountSystem::login_user;
}

void AccountSystem::userdataUpdate() {
    
    try {
        std::ofstream info_out(AccountSystem::USER_DATA_PATH);
        if( !info_out ) {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        for (size_t i = 0; i < user_list.size(); ++i) {
            info_out << user_list[i].getUsername() << "," << user_list[i].getPassword();
            if (i != user_list.size() - 1) {
                info_out << "\n";
            }
        }

        info_out.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
}
