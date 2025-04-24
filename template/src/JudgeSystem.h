#include <iostream>
#include "AccountSystem.h"
#include "Problem.h"
#include "MainPage.h"
class JudgeSystem : private AccountSystem, private ProblemSystem, private MainPage {
    private:
        std::string USER_DATA_PATH;
        std::string PROBLEM_DATA_PATH;
        std::string LOGIN_MSG_PATH;
        std::string VERSION;
        std::string status;
        void loadData();
        void effectLoading(std::string content);
    public:
        JudgeSystem() = default;
        JudgeSystem(std::string userPath, std::string problemPath, std::string msgPath, std::string version);
        void initSystem();
        int mainPage();
        std::string getUserpath() const { return USER_DATA_PATH; }
        std::string getProblempath() const { return PROBLEM_DATA_PATH; }
        std::string getMsgpath() const { return LOGIN_MSG_PATH; }
        std::string getVersion() const { return VERSION; }
};