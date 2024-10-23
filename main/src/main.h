#include <vector>
#include <iostream>
#include <utility>
#include <set>

template<typename T>
using matrix2D = std::vector<std::vector<T>>;

using coordinates = std::pair<int, int>;
using indexed_set_container = std::vector<std::set<coordinates>>;

struct Student {
    // true for greedy, false for not
    bool greedy;
    // -1 for no preference, 0-row count-1 for a specific preference
    int preferred_row;
    // 0-fullness-1
    int ID;
    // boolean for sitting or standing
    bool sitting;
    // integer for the payoff amount
    int payoff;
    int max_utility;
    Student(int greedy, int preferred_row, int ID, bool sitting, int payoff, int max_utility)
        : greedy(greedy), preferred_row(preferred_row), ID(ID), sitting(sitting), payoff(payoff), max_utility(max_utility) {};
    void printStudent(){
        std::cout<< "Student ";
        if (ID < 10){std::cout << "0";}
        std::cout << ID << " ";
        if (sitting){
            std::cout << "has   been  sat, ";
        } else {
            std::cout << "hasn't been sat, ";
        }
        // if (greedy){
        //     std::cout << "is  greedy,  ";
        // } else {
        //     std::cout << "is ungreedy, ";
        // }
        if (payoff == -1){
            std::cout << "and has no payoff\n";
        } else {
            std::cout << "and has payout: " << payoff << "\n";
            std::cout << "and has preferred maximum utility: " << max_utility << "\n";
        }
    }
};

struct Classroom {

    int seat_num;
    int row_count;
    int col_count;
    int fullness;
    int number_sat;
    bool naive;

    matrix2D<int> rows;
    matrix2D<int> closest_student_dist;
    std::vector<Student> students;
    indexed_set_container rows_mapped_by_payoff;
    // std::vector<int> payoffs;

    Classroom(int seat_num, int row_count, int fullness, bool naive_, int util_) ;
    void printClassroom();
    void printStudents();
    void sitStudent();
    void sitAllStudents(bool debug);
    void reCalcDistances(int row_num);
    void reCalcPayoffs(int row_num);
    void printDistances();
};

class Game{
private:
    Classroom room;
    std::vector<Student> students;
    int num_sat;
public:
    void display();
    void sitOne();
    void sitAll();
    Game(Classroom room, Student);

};
