#include <iostream>
#include <vector>
#include <utility>
#include "User.h"

class AccountSystem : private User {
    private:
        std::vector<User> user_list;
        std::string login_user;
        std::string USER_DATA_PATH;
        void sign_up();
        void userdataUpdate();
    protected:
        void init(std::string USER_DATA_PATH);
        User* search(std::string name);
        std::pair<bool, std::string> login();
        void adding_user(std::string name, std::string password);
        std::string getuserLogin();
};