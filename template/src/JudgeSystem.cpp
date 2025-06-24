#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cctype>
#include <climits>
#include <thread>
#include <ctime>
#include <functional>  // for std::hash
#include <algorithm>   // for std::sort
#include "JudgeSystem.h"
#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

void clearWindow() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    return;
}
JudgeSystem::JudgeSystem(std::string userPath, std::string problemPath, std::string msgPath, std::string version) {
    USER_DATA_PATH = userPath;
    PROBLEM_DATA_PATH = problemPath;
    LOGIN_MSG_PATH = msgPath;
    VERSION = version;
    status = "NOT READY";
}
void JudgeSystem::effectLoading(std::string content) {
    static const char* spinner[] = {"|", "/", "-", "\\"};
    for (int i = 0, j = 0; j < 20; j++) { 
        std::cout << "\033[33m" << content << "\033[0m" << spinner[i] << "\r";
        std::flush(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        i = ( i + 1 ) % 4;
    }
    return;
}
void JudgeSystem::loadData() {

    // Step1: Call AccountSystem::init with user data path
    AccountSystem::init(JudgeSystem::USER_DATA_PATH);
    JudgeSystem::effectLoading("Status - Loading user data...");
    std::cout << yellow("Status - Loading user data...") << green("OK!\n");

    // Step2: Call ProblemSystem::init with problem data path
    ProblemSystem::init(JudgeSystem::PROBLEM_DATA_PATH);
    JudgeSystem::effectLoading("Status - Loading problem data...");
    std::cout << yellow("Status - Loading problem data...") << green("OK!\n");

    // Step3: welcome the user
    try {
        std::ifstream inputFile("./msg/login.txt");
        if( !inputFile ) {
            throw std::runtime_error("Error: File does not exist - ./msg/login.tx");
        }

        std::string line;
        std::cout << "\033[36m";
        while (std::getline(inputFile, line)) {
            std::cout << line;
            std::cout << "\n";
        }
        inputFile.close(); 
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    return;
}
int JudgeSystem::mainPage() {
    if( JudgeSystem::status == "NOT READY" ) {
        loadData();
        JudgeSystem::status = "USER LOGIN";
        return 0;
    }
    else if( JudgeSystem::status == "USER LOGIN" ) {
        std::pair<bool, std::string> user_info = AccountSystem::login();
        if( user_info.first == false ) return 0;
        JudgeSystem::status = "READY";
        return 0;
    }
    
    MainPage::mainpagePrint();
    int opt = operationCheck();

    clearWindow();

    switch (opt) {
        case -1:
            std::cout << red("Invalid input, please enter a number from 1 to 7.\n");
            break;

        case 1:
            std::cout << green("Username: ") << getuserLogin() << "\n";
            break;

        case 2:
            std::cout << green("VERSION: ") << getVersion() << "\n";
            break;
        case 3: {
        std::cout << cyan("[Problem List]\n");
        auto problems = list_problem();
        for (size_t i = 0; i < problems->size(); ++i) {
            std::cout << "Problem " << char('A'+i) << ": " << (*problems)[i].getTitle() << "\n";
        }
        break;
        }
        case 4:
            randomDailyProblem();
            break;
        case 5:
            submitSolution();
            break;
        case 6:
            newproblem_set(PROBLEM_DATA_PATH);
            break;
        case 7:
            return -1;
            break;
        default:
            std::cout << yellow("This operation is not implemented yet.\n");
            break;
    }
    return 0;
}

void JudgeSystem::submitSolution()
{
    // 1. 讀題目 ID & 程式路徑
    std::string probID, codePath;
    std::cout << "Please input the problem ID: ";
    std::cin >> probID;
    std::cout << "Please input your code path: ";
    std::cin >> codePath;
    std::cin.ignore(); // 清理輸入緩衝區

    // 2. 編譯
    const std::string exe = "tmp_exec";
    std::string compileCommand = "g++ -std=c++17 -o " + exe + " \"" + codePath + "\"";
    //std::cout << "DEBUG: 編譯指令: " << compileCommand << std::endl;
    if (std::system(compileCommand.c_str()) != 0) {
        std::cout << red("Compiler Error\n");
        return;
    }

    // 3. 找題目的 input/output
    std::string inPath, outPath;
    auto plist = list_problem(); // 假設 list_problem() 正常工作
    bool found = false;
    if (plist) { // 檢查 plist 是否為空
        for (auto &p : *plist) {
            if (p.getTitle() == probID) {
                inPath  = p.getInput();
                outPath = p.getOutput();
                found   = true;
                break;
            }
        }
    }
    if (!found) {
        std::cout << red("Problem ID not found\n");
        return;
    }
    std::cout << blue("Input path: ") << inPath << std::endl;
    std::cout << blue("Output path: ") << outPath << std::endl;


    // 4. 執行並重導出（新增執行結果判斷）
    // 在 Windows 環境下，執行檔通常會有 .exe 副檔名，確保包含
    // 對於路徑包含空格的情況，使用雙引號包裹
    std::string runCommand = std::string("cmd /c \"") + exe + ".exe\" < \"" + inPath + "\" > user_output.txt";
    //std::cout << "DEBUG: 執行指令: " << runCommand << std::endl; // 新增偵錯輸出

    int runResult = std::system(runCommand.c_str());

    // 判斷執行結果
    if (runResult != 0) {
        std::cout << red("Runtime Error or Execution Failed\n");
        //std::cout << "DEBUG: 執行結果代碼 (runResult): " << runResult << std::endl; // 顯示具體錯誤碼
        return;
    }
    //std::cout << green("Execution successful.\n"); // 執行成功提示

    // 5. 比對輸出
    // 檢查檔案是否成功開啟
    std::ifstream usr("user_output.txt");
    std::ifstream corr(outPath);

    if (!usr.is_open()) {
        std::cout << red("Error: Could not open user_output.txt for comparison. It might not have been generated or path is wrong.\n");
        return;
    }
    if (!corr.is_open()) {
        std::cout << red("Error: Could not open correct output file (") << outPath << ") for comparison. Check problem data.\n";
        return;
    }

    std::string uline, cline;
    bool ok = true;
    int line_num = 0; // 新增行號，方便偵錯

    while (std::getline(corr, cline)) {
        line_num++;
        //std::cout << "DEBUG: 比對第 " << line_num << " 行 - 正確答案: [" << cline << "]" << std::endl; // 偵錯輸出
        if (!std::getline(usr, uline)) {
            // 用戶輸出提前結束
            ok = false;
            std::cout << red("Wrong Answer: User output ended prematurely.\n"); // 偵錯輸出
            break;
        }
        //std::cout << "DEBUG: 比對第 " << line_num << " 行 - 用戶輸出:   [" << uline << "]" << std::endl; // 偵錯輸出

        // 這裡可以考慮對比對進行一些容錯處理，例如去除行末空白符
        // 例如：trim(cline); trim(uline);
        if (cline != uline) {
            ok = false;
            std::cout << red("Wrong Answer: Mismatch at line ") << line_num << std::endl; // 偵錯輸出
            break;
        }
    }

    // 檢查用戶輸出是否有多餘的行
    if (ok && std::getline(usr, uline)) {
        ok = false;
        std::cout << red("Wrong Answer: User output has extra lines.\n"); // 偵錯輸出
    }

    // 關閉文件流
    usr.close();
    corr.close();

    // 6. 輸出結果
    std::cout << blue("Judge result: ")
              << (ok ? green("Accepted\n") : red("Wrong Answer\n"));

    // 清理生成的臨時文件 (可選，但推薦在實際評測系統中使用)
    // std::filesystem::remove(exe + ".exe");
    // std::filesystem::remove("user_output.txt");
}


void JudgeSystem::randomDailyProblem() {
    auto plist = list_problem();
    if (plist->empty()) {
        std::cout << red("No problems available.\n");
        return;
    }

    // 1. 取得今天日期 YYYYMMDD
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    char dateStr[9];
    std::strftime(dateStr, sizeof(dateStr), "%Y%m%d", now);

    // 2. 用日期 + magic 做 hash，存成 pair(hash, index)
    std::vector<std::pair<size_t, size_t>> hashes; 
    for (size_t i = 0; i < plist->size(); ++i) {
        const auto& prob = (*plist)[i];
        std::string key = std::string(dateStr) + std::to_string(prob.getNum());
        size_t h = std::hash<std::string>{}(key);
        hashes.emplace_back(h, i);
    }

    // 3. 依 hash 值排序
    std::sort(hashes.begin(), hashes.end(),
              [](auto &a, auto &b){ return a.first < b.first; });

    // 4. 取第一個 hash mod 題目數量（也可直接取 hashes[0].second）
    size_t pickIndex = hashes[0].second;

    // 5. 顯示結果
    const auto& todayProb = (*plist)[pickIndex];

    // 題目說明 map
    std::unordered_map<std::string, std::string> descriptions = {
        {"A+B", "Given two integers A and B, output their sum."},
        {"A-B", "Given two integers A and B, output the result of A minus B."},
        {"GCD", "Given two integers, compute and output their greatest common divisor (GCD)."},
        {"LCM", "Given two integers, compute and output their least common multiple (LCM)."}
    };

    std::cout << blue("=== Today's Daily Problem ===\n");
    std::cout << "Title       : " << todayProb.getTitle()     << "\n";

    // 顯示對應說明（如果有）
    auto it = descriptions.find(todayProb.getTitle());
    if (it != descriptions.end()) {
        std::cout << "Description : " << it->second << "\n";
    } else {
        std::cout << "Description : (No description available)\n";
    }

    std::cout << "Input Path  : " << todayProb.getInput()  << "\n";
    std::cout << "Output Path : " << todayProb.getOutput() << "\n";
    std::cout << "Magic Number: " << todayProb.getNum()     << "\n";
    std::cout << green("You can now go back and use option (5) to submit your solution.\n");
}
