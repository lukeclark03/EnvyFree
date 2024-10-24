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
            std::cout << "and has payout: " << payoff << std::endl;
            // std::cout << "and has preferred maximum utility: " << max_utility << "\n";
        }
    }

    // std::vector<int> withoutSelfDistances(std::vector<int>& current_row, std::vector<int>& current_row_distances) {
        
    //     std::vector<int> selfdist(current_row_distances);
    //     // Add case where set where seat is more than 1 -> no change in vector
    //     // return current_row;
    //     auto found = find(current_row.begin(),current_row.end(), this->ID);

    //     auto before = current_row.begin();
    //     for (auto it = current_row.begin(); it != found; it++) {
    //         if (*it != -1)
    //             before = it;
    //     }

    //     auto after = found;
    //     for (auto it = found; it != current_row.end(); it++) {
    //         if (*it != -1)
    //             after = it;
    //     }

    //     int dist_before = distance(before, found);
    //     int dist_after = distance(found, after);
    //     int index = distance(current_row.begin(), found);

    //     // Case 1: Surrounded 1 (1) 1
    //     if (dist_before == 0 && dist_after == 0)
    //         selfdist.at(index) = 1;
        
    //     // Case 2: One next, one N away
    //         // two symmetrical conditions
    //         // Need to add 1 to middle of N dist, from middle up to self
        
    //     // Case 3:
    //         // One M away, one N away
    //         // similar to above, but need to treat self as middle, incrementing from previous 2 middle values
    //         // until now middle value

    //     // Case 4:
    //         // One of the edges is free
    //         // Just keep adding past middle for other person

    //     // Case 5:
    //         /// Only one in row
    //         // Just take each value to be row length

    // }

};

struct Classroom {

    int seat_num;
    int row_count;
    int col_count;
    int fullness;
    int number_sat;
    bool naive;
    int max_utility;

    matrix2D<int> rows;
    matrix2D<int> closest_student_dist;
    std:: vector<std::vector<std::vector<Student*>>> layout;
    std::vector<Student*> students;
    indexed_set_container rows_mapped_by_payoff;
    // std::vector<int> payoffs;

    Classroom(int seat_num, int row_count, int fullness, bool naive_, int max_util) ;
    void printClassroom();
    void printStudents();
    void sitStudent();
    void sitAllStudents(bool debug);
    void reCalcDistances(int row_num);
    void reCalcPayoffs(int row_num);
    void printDistances();
    int getUnhappiestStudent();
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
