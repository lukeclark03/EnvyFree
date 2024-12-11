#include <iterator>
#include <vector>
#include <iostream>
#include <utility>
#include <set>
#include <map>
#include <algorithm>
#include <tuple>


template<typename T>
using matrix2D = std::vector<std::vector<T>>;

using coordinates = std::pair<int, int>;
using indexed_set_container = std::vector<std::set<coordinates>>;
using Coalition = std::tuple<std::set<int>, std::map<std::set<std::pair<int, int>>, std::map<int, double>>, std::map<int, double>>;
// coalition has members, newpositions and corresponding maps, and total coalition map

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
    //     struct Coalition{
    //     std::set<int> members;
    //     // members is the set of members in the coalition
    //     std::map<std::set<std::pair<int, int>>, std::map<int, double>> repositioning;
    //     // repositioning is a map, with key being a set of locations that are a maximal reseating for this coalition, and values being the payoff map from student IDs to payoffs, which may be decimal because there is a random permutation chosen: ie all players in the coalition receive the same payoff.
    //     std::map<int, double> expectedPayoffs;
    //     // expectedPayoffs are the expected payoffs calculated for all players, with a distribution over the seats and permutations the coalition deems best.
    //     // coalitions choose seats based on the highest total payoff.


    //     // std::set<std::pair<,>>

    // };


    struct Partition{

        Partition(std::set<Coalition> coalitions_){
            // setting numbStudents, since the partition is not
            coalitions = coalitions_;
            numStudents = std::get<2>(*coalitions.begin()).size();
            // std:: cout << numStudents;
            // throw std::exception();
            // std::cout << "seeding expected payoffs map to 0" << std::endl;
            for(int i = 0; i < numStudents; i++){
                // std::cout << "adding " << i << " , 0 to the map" << std::endl;
                studentExpectedPayoffs[i] = 0;
            }

            // std::cout << "adding the expected payoffs from each coalition" << std::endl;
            for(Coalition coalition : coalitions){
                coalition_IDs.insert(std::get<0>(coalition));
                // std:: cout << "next coalition" << std::endl;
                for(int i = 0; i < numStudents; i++){

                    studentExpectedPayoffs[i] += std::get<2>(coalition)[i] * (double(std::get<0>(coalition).size()) / double(numStudents));
                }
            }

        }
        // std::map<int, double> calculateExpectedPayoffs(std::set<Coalition> coalitions);
        void printPartition(){
            std::cout << "Printing partition below, "<< numStudents << " students" << std::endl;
            int i = 0;
            for (Coalition coalition : coalitions){
                std::cout << "Coalition " << i <<": ";
                for(int member : std::get<0>(coalition)){
                    std::cout << member << "  ";
                }
                std::cout << std::endl;
                i++;
            }

            std::cout << "Payoff map:\n";
            for (const auto& pair : studentExpectedPayoffs) {
                std::cout << "ID: " << pair.first << ", Payoff: " << pair.second << "\n";
            }

        }

        void printAdjacencies() {
            std::cout << std::endl <<  "Printing adjacent partitions" << std::endl;
            int i = 0;
            for (auto it = adjacencies.begin();  it != adjacencies.end(); it++ ) {
                std::cout << std::endl << "\t Adjacency " << i << "contains the set of students: ";
                std::cout << "{ ";
                for (int elem : it->first) {
                    std::cout << elem << " ";
                }
                std::cout << "} " << std::endl << std::endl;
                i++;
            }
            if (adjacencies.empty())
                std::cout << "Could not print adjacents." << std::endl;
        }

        std::set<Coalition> coalitions;
        std::set<std::set<int>> coalition_IDs;
        int numStudents;
        std::map<int, double> studentExpectedPayoffs;
        // std::set<Partition*> adjacencies;
        std::map<std::set<int>, Partition*> adjacencies;
        std::set<Partition*> weak_adjacents;
        std::set<Partition*> strong_adjacents;





    };


    struct Round{
        // a Round stores the current state of the game, including all possible coalitions and partitions

        std::map<std::set<int>, Coalition> coalitionMap;
        // This  is all of the coalitions precalculated to make the parititions quicker
        std::vector<Partition*> Partitions;
        // this is all of the partitions of the current game, and they should be linked properly.
        std::map<std::set<std::set<int>>, Partition*> partitionMap;
        // This is a map to more easily access the particular partition pointer.
        // int numStudents;
        // void addToCoalitionMap(std::set<int> IDs);
        // void createParitions();


    };

    int seat_num;
    int row_count;
    int col_count;
    int fullness;
    int number_sat;
    int max_utility;
    bool print_mode;

    matrix2D<int> rows;
    matrix2D<int> closest_student_dist;
    std::vector<std::vector<std::vector<Student*>>> layout;
    std::vector<Student*> students;
    std::set<Coalition> coalitions;
    Round round;
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
    void placeStudent(int ID, std::pair<int, int> loc);
    void moveStudents(std::map<int, std::pair<int, int>> newPositions);
    std::set<std::pair<int, int>> getEmptySeats();
    std::pair<std::set<std::set<std::pair<int, int>>>, int> getBestReseating(std::set<int> IDs);
    std::map<int, std::pair<int, int>> makeReseatMap(std::set<int> IDs, std::set<std::pair<int, int>> locations);

    std::set<std::set<std::pair<int, int>>> getCombinationsOf(std::set<std::pair<int, int>> empties_, int number_);

    Coalition createCoalition(std::set<int> IDs);

    void addToCoalitionMap(std::set<int> IDs);
    void createPartitions();
    std::set<std::set<int>> breakPartition(std::set<std::set<int>> partition, std::set<int> defectors);
    void fillAdjacencies(Partition* p);
    void fillAllAdjacencies();
    void thinAdjacencies(Partition* p);
    void thinAllAdjacencies();
    void printPartitions();


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
