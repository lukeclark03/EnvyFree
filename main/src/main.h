#include <iterator>
#include <vector>
#include <iostream>
#include <utility>
#include <set>
#include <algorithm>



template<typename T>
using matrix2D = std::vector<std::vector<T>>;

using coordinates = std::pair<int, int>;
using indexed_set_container = std::vector<std::set<coordinates>>;

struct Student {
    // -1 for no preference, 0-row count-1 for a specific preference
    int preferred_row;
    // 0-fullness-1
    int ID;
    // boolean for sitting or standing
    bool sitting;
    // integer for the payoff amount
    int payoff;
    int max_utility;
    int row = -1;
    int col = -1;

    Student( int preferred_row, int ID, bool sitting, int payoff, int max_utility)
        :preferred_row(preferred_row), ID(ID), sitting(sitting), payoff(payoff), max_utility(max_utility) {};
    void printStudent(){
        std::cout<< "Student ";
        if (ID < 10){std::cout << "0";}
        std::cout << ID << " ";
        if (sitting){
            std::cout << "has been sat at row " << row << " and column " << col << ", ";
        } else {
            std::cout << "hasn't been sat, ";
        }
        // if (greedy){
        //     std::cout << "is  greedy,  ";
        // } else {
        //     std::cout << "is ungreedy, ";
        // }
        if (payoff == -1){
            std::cout << "and has no payoff" << std::endl;
        } else {
            std::cout << "and has payoff: " << payoff << std::endl;
            // Implemented per student if we want to include personal space as a factor, for example
            // std::cout << "and has preferred maximum utility: " << max_utility << "\n";  
        }
    }

    //  Keep for future if needed
    std::vector<int> withoutSelfDistances(std::vector<int>& current_row, std::vector<int>& current_row_distances) {
        
        std::vector<int> selfdist(current_row_distances);
        // Add case where set where seat is more than 1 -> no change in vector
        // return current_row;
        auto found = find(current_row.begin(),current_row.end(), this->ID);

        auto before = current_row.begin();
        for (auto it = current_row.begin(); it != found; it++) {
            if (*it != -1)
                before = it;
        }

        auto after = found;
        for (auto it = found; it != current_row.end(); it++) {
            if (*it != -1)
                after = it;
        }

        int dist_before = std::distance(before, found);
        int dist_after = std::distance(found, after);
        int index = std::distance(current_row.begin(), found);

        // Case 1: Surrounded 1 (1) 1
        if (dist_before == 0 && dist_after == 0)
            selfdist.at(index) = 1;
        
        // Case 2:
            // One M away, one N away
            // redo by iterating from indicies of each side
        else if (before != current_row.begin() && after != found) {// They are not the edges, which would mean they were never changed.
            int bef_index = index - dist_before;
            int aft_index = index + dist_after;

            for (int i = bef_index + 1; i < aft_index; i++) {
                selfdist[i] = std::min(i - bef_index, aft_index - i);
            }

        }
        // Case 3:
            // One of the edges is free
            // Just keep from previous middle  past middle for other person
        else if (before == current_row.begin() && after != found) {
            int aft_index = index + dist_after;
            for (int i = 0; i < index + aft_index; i++) {
                selfdist[i] = aft_index - i;
            }
        } else if (before != current_row.begin() && after == found) {
            for (int i = index + 1; i < selfdist.size(); i++) {
                selfdist[i] =  i - index;
            }
        }
        // Case 4:
            /// Only one in row
            // Just take each value to be row length
        else {
            selfdist = std::vector<int>(selfdist.size(), selfdist.size());
        }

        return selfdist;

    }

};

struct Classroom {

    int seat_num;
    int row_count;
    int col_count;
    int fullness;
    int number_sat;
    int max_utility;
    bool print_mode;

    matrix2D<int> rows;
    matrix2D<int> closest_student_dist;
    std:: vector<std::vector<std::vector<Student*>>> layout;
    std::vector<Student*> students;
    //indexed_set_container rows_mapped_by_payoff; //unused for now
    // std::vector<int> payoffs;

    Classroom(int seat_num, int row_count, int fullness, int max_util, bool print_mode) ;
    ~Classroom();
    void printClassroom();
    void printStudents();
    void whoCanImprove();

    void sitStudent();
    void sitStudentAt(int row, int col);
    void sitAllStudents(bool debug);
    void sitAllStudentsNash();
    void reCalcDistances(int row_num);
    void reCalcPayoffs(int row_num);
    void printDistances();
    int getUnhappiestStudent();
    void removeStudent(int ID);
    void moveStudent(int ID);
    bool canImprove(int ID);
    bool bestResponse();
    int iteratedBestResponse();
    std::vector<int> getStudentUtilities();

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
