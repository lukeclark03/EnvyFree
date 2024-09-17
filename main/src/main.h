#include<vector>
#include <iostream>
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
    Student(int greedy, int preferred_row, int ID, bool sitting, int payoff) : greedy(greedy), preferred_row(preferred_row), ID(ID), sitting(sitting), payoff(payoff) {};
    void printStudent(){
        std::cout<< "Student " << ID << " ";
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
            std::cout << "and has no payout\n";
        } else {
            std::cout << "and has payout: " << payoff << "\n";
        }
    }
};

struct Classroom {

    int seat_num;
    int row_count;
    int col_count;
    int fullness;
    int number_sat;
    std::vector<std::vector<int>> rows;
    std::vector<std::vector<int>> closest_student_dist;
    std::vector<Student> students;
    // std::vector<int> payoffs;

    Classroom(int seat_num, int row_count, int fullness) ;
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