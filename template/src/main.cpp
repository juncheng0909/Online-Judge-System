#include <iostream>
#include "JudgeSystem.h"
#define blue(text)   "\033[34m" text "\033[0m"

#define USER_DATA_PATH "./data/user/user.csv"
#define PROBLEM_DATA_PATH  "./data/problem/problem.csv"
#define LOGIN_MSG_PATH "./msg/login.txt"
#define VERSION "1.0.0"

int main() {

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    JudgeSystem judge(USER_DATA_PATH, PROBLEM_DATA_PATH, LOGIN_MSG_PATH, VERSION);
    std::cout << blue("Simple Judge System start! Version: ") << VERSION << "\n";

    int result = 0;
    
    do {
        result = judge.mainPage();
    }while( result == 0 );

    return 0;
}
