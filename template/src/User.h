#include <iostream>
#include <vector>
class User {
    private:
        std::string username;
        std::string password;
    public:
        User() = default;
        User(std::string name, std::string passwd);

        std::string getUsername() const { return username; }
        std::string getPassword() const { return password; }
};
