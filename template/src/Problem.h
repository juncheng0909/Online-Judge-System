#include <iostream>

class Problem {
    protected:
        std::string problem_title;
        std::string input_path;
        std::string output_path;
        int magic_number;
    public:
        Problem() = default;
        Problem(std::string title, std::string input, std::string output, int magic_num);
        std::string getTitle() const { return problem_title; }
};
class ProblemSystem : public Problem {
    private:
        std::vector<Problem> problem_list;
        void adding_problem(Problem new_problem);
    public:
        void init(std::string &PROBLEM_DATA_PATH);
        std::vector<Problem>* list_problem();
        void newproblem_set();
};