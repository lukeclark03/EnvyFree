#include "testing.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <unordered_set>

using namespace std;


// std::map<int, double> Partition::calculateExpectedPayoffs(std::set<Coalition> coalitions){

//     for(int i = 0; i < numStudents; i++){
//         studentExpectedPayoffs[i] = 0;
//     }

//     for(auto coalition : coalitions){
//         for(int i = 0; i < numStudents; i++){
        
//             studentExpectedPayoffs[i] += coalition.expectedPayoffs[i] * (double(coalition.members.size()) / double(numStudents));
//         }
//     }
// }

vector<Student> initStudents(int num_students, int num_greedy){
    vector<Student> students;

    for(int i = 0; i < num_students; i++){
        // For now, does nothing else to initialize students,
        // May implement personalities here.
    }

    return students;
}

void fillrows(int num_students){
    // put a student into a row



    // call display function to portray all rows with new student
}


Classroom::Classroom(int seat_num_, int row_count_, int fullness_,int util_, bool print_mode_){

    // Initialization of member variables
    this->seat_num = seat_num_;
    this->row_count = row_count_;
    this->fullness = fullness_;
    max_utility = util_;
    print_mode = print_mode_;

    // Validity Checks
    if (seat_num % row_count != 0){
        throw invalid_argument("Seat count must be a multiple of row count.");
    } else if (fullness > seat_num){
        throw invalid_argument("Must have fewer students than seats.");
    }

    // Setup columns, initialize seated variable
    col_count = seat_num/row_count;
    number_sat = 0;

    // initialize the grid of students in the classroom (-1 is empty, 0-N are used by student with that number)
    
    vector<int> temp_row;
    for (int i = 0; i < row_count; i++){
        temp_row.clear();
        for (int j = 0; j < col_count; j++){
            temp_row.push_back(-1);
        }
        rows.push_back(temp_row);
    }

    // initialize the grid of distances for the students
    temp_row.clear();
    for (int i = 0; i < row_count; i++){
        temp_row.clear();
        for (int j = 0; j < col_count; j++){
            temp_row.push_back(max_utility);
        }
        closest_student_dist.push_back(temp_row);
    }



    // initialize the layout of the classroom with empty vectors
    vector<Student*> temp_ptr_list;
    vector<vector<Student*>> temp_ptr_row;
    for (int i = 0; i < row_count; i++){
        temp_ptr_row.clear();
        for (int j = 0; j < col_count; j++){
            temp_ptr_row.push_back(temp_ptr_list);
        }
        layout.push_back(temp_ptr_row);
    }


    
    // for (int i = 0; i < row_count; i++){
    //     temp_row.clear();
    //     for (int j = 0; j < col_count; j++){
    //         temp_row.push_back(col_count);

    //         // fill in payoffs as all at 0 payoff, so the first iteration is all random.
    //         rows_mapped_by_payoff.back().emplace(coordinates(i,j));
    //     }
    //     closest_student_dist.push_back(temp_row);
    // }

    // Students are greedy/nongreedy depending on the class variable
    
    for (int i = 0; i < fullness; i++){
        // Adding students to the vector of students
        // at this point, all students are greedy (true) and none of them have a preferred row (-1)
        Student* new_student = new Student(-1, i, false, -1, max_utility);  
        students.push_back(new_student);
    }
    // printStudents();

}

Classroom::~Classroom(){
    for (int i = 0; i < students.size(); i++ ){
        delete students[i];
    }
}

vector<int> Classroom::getStudentUtilities(){
        vector<int> utilities(4);

        for(auto student : students){
            utilities[student->payoff]++;
        }

        return utilities;
    }


void Classroom::sitStudentAt(int row, int col){
    if (number_sat >= fullness){
        throw invalid_argument("Cannot sit another student, everyone has been sat");
    }
    layout[row][col].push_back(students[number_sat]);
    students[number_sat]->sitting = true;
    students[number_sat]->row = row;
    students[number_sat]->col = col;
    number_sat++;
    reCalcDistances(row);
}


void Classroom::sitAllStudentsNash(){
    for (int i = 0; i < row_count; i++){
        // for loop for each row, we begin sitting students inwards
        int dist_from_edge = 0;
        // while loop for seating them from the edges inwards
        if (number_sat >= fullness){
            return;
        }
        while (dist_from_edge/3 < (col_count + 2)/6){
            // we can seat a student at dist_from edge on both sides and keep everyone getting 3
            sitStudentAt(i, dist_from_edge);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }
            sitStudentAt(i, col_count - dist_from_edge -1);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }
            reCalcPayoffs(i);

            //increment the number of students that have been sat
            dist_from_edge += 3;
            // cout << "incremented edge distance" << endl;
        }
        // reCalcPayoffs
    }
    // we have sat all students on the edge, now we deal with the middle
    for (int i = 0; i < row_count; i++){
        if (col_count % 6 == 1){
            // there is a middle spot with utility of 3 that doesn't harm anyone
            sitStudentAt(i, col_count/2);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }
        } else if (col_count % 6 == 2){
            // there are two middle seats, one gets the acutal middle, one gets one closed to the edge
            sitStudentAt(i, col_count/2);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }

            sitStudentAt(i, col_count/2 -2);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }
        } else if (col_count % 6 == 3){
            sitStudentAt(i, col_count/2 +1);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }

            sitStudentAt(i, col_count/2 -1);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }
        } else if (col_count % 6 == 4){
            // don't do anything, they are sat well
        } else if (col_count % 6 == 5){
            // sit them in the middle spot
            sitStudentAt(i, col_count/2);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            }
        } else if (col_count % 6 == 0){
            // sit them in the middle spot
            sitStudentAt(i, col_count/2);
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            } 
        }
        reCalcPayoffs(i);
    }

    // now we still have students, so some people are going to get a utility of 1
    for(int i = 0; i < row_count; i++){
        for (int j = 0; j < col_count; j++){
            if (layout[i][j].size() == 0){
                sitStudentAt(i, j);
            }
            if (number_sat >= fullness){
                reCalcPayoffs(i);
                return;
            } 
        }
        reCalcPayoffs(i);
    }
}



void Classroom::sitAllStudents(bool debug){
    // this function seats all the students in the classroom, one at a time, with or without debugging
    // seed the random number generator
   
    
    if (debug){
        printClassroom();
        printStudents();
    }
    for(int i = 0; i < fullness; i ++){
        sitStudent();
        if (debug){
            printClassroom();
        }
    }
    if (debug){    printStudents();}

}

void Classroom::sitStudent(){
    // This function seats a single student, the next in the list

    // Validity Checking
    if (number_sat >= fullness){
        throw invalid_argument("Cannot sit another student, everyone has been sat");
    }
    if (students[number_sat]->sitting){
        throw invalid_argument("Cannot sit a student that is already seated");
    }

    // get the current student who needs to sit
    Student* toSit = students[number_sat];
    // cout << "sitting a student: " << toSit->ID << endl;
    
    std::mt19937_64 twister(
        (mt19937_64::result_type)
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
    std::uniform_int_distribution<> randgen_row{0,row_count - 1 };
    std::uniform_int_distribution<> randgen_col{0,col_count - 1 };
    // get a random row
    int rand_row = randgen_row(twister);

    // cout << "Random Row: " << rand_row << endl;
    // get a random column
    int rand_col = randgen_col(twister);

    // cout << "Random col: " << rand_col << endl;

    if (print_mode){cout << "sitting student randomly at row " << rand_row << " and col: " << rand_col << endl;}
    // add the student to that spot
    // toSit->printStudent();
    layout[rand_row][rand_col].push_back(toSit);

    // recalculate the distances and payoffs for the row the student sat in
    reCalcDistances(rand_row);
    reCalcPayoffs(rand_row);
    // cout << "finished reCalcDsitances" << endl;
    // reCalcPayoffs(rand_row, toSit);

    //increment the number of students that have been sat
    number_sat++;
    toSit->sitting = true;
    toSit->row = rand_row;
    toSit->col = rand_col;
    // cout << "finished sitting students" << endl;

}

void Classroom::reCalcDistances(int row_num){
    // This function updates the distances to the nearest student after a person has been seated
    // we will be using layout[row_num], as it is the only row that has been changed
    
    // first, iterate through all the seats in that row, recalculating the payoff at each one
    for (int i =0; i < col_count; i++){
        // cout << "examining row " << row_num << " col: " << i << endl;
        if (layout[row_num][i].size() > 1){
            // cout <<" more than 2 students in seat " << endl;
            // there are more than 2 students in this seat. The distance is 0
            closest_student_dist[row_num][i] = 0;
            
        } else {
            // cout << "less than 2 students in seat" << endl;
            // This seat is occupied by one student, or by none. The calculations are the same for either (distance is at least 1)
            // distance to student on the left
            int l_dist = 1;
            // distance to the student on the right
            int r_dist = 1;
            // boolean for the space remaining, to calculate it for the loop
            bool space_remains = true;
            // while loop determines how much space is to the left of the seat i
            while (space_remains){
                if(i - l_dist < 0){
                    //  spot i-l_dist is a wall, so the l_dist is maximum, and we break loop
                    l_dist = max_utility;
                    space_remains = false;
                } else {
                    // we have not reached a wall, so there is a valid space to the left, we must check if it is occupied or not
                    if(layout[row_num][i-l_dist].size() > 0){
                        // there is a person on space i-l_dist
                        space_remains = false;

                    } else {
                        // there is not a person on the left, we increment l_dist unless it is already the max
                        l_dist++;
                        if (l_dist >= max_utility){
                            space_remains = false;
                        }
                        
                    }
                }
            }
            // cout << "got l_dist to be " << l_dist;

            space_remains = true;
            // while loop to determine how much space is to the right of seat i
            while(space_remains){
                if (i+r_dist >= col_count){
                    // the spot i + r_dist is a wall, so the r_dist is maximum and we break
                    r_dist = max_utility;
                    space_remains = false;
                } else {
                    // there is a spot at i + r_dist
                    // is there a person at that spot?
                    if (layout[row_num][i+r_dist].size() > 0){
                        // there is a person on space i + r_dist, so the r_dist is maximum. we break loop
                        space_remains = false;
                    } else {
                        // there is not a person on the space i + r_dist, so we increment the r_dist unless it is already the max
                        r_dist ++;
                        if (r_dist >= max_utility){
                            // we already have max space on the right, so we break loop
                            space_remains = false;
                        }
                    }
                }
            }
            // cout << "got r_dist to be " << r_dist << endl;

            // we now have the distances to the nearest player on left: l_dist, and on right: r_dist
            // the distance is the min of these
            // cout << min(max_utility, min(l_dist, r_dist)) << endl;
            // cout << closest_student_dist.size();
            // cout << closest_student_dist[0].size();
            closest_student_dist[row_num][i] = min(max_utility, min(l_dist, r_dist));
            // cout << "assigned the utility to be " << closest_student_dist[row_num][i] << endl;
        }
    }

}


void Classroom::reCalcPayoffs(int row_num){
    // we will be working with layout[row_num] and closest_student_dist[row_num]
    // cout << "recalculating payoffs" << endl;

    // now we go through the entire layout and assign to each person at a spot the payoff of that spot
    for (int i = 0; i < col_count; i++){
        // we are considering cell layout[row_num][i]
        for (int j =0; j < layout[row_num][i].size(); j++){
            // we consider each person seated at that row
            layout[row_num][i][j]->payoff = closest_student_dist[row_num][i];
        }
    }


    for (int i = 0; i < col_count; i++){

        int index = rows[row_num][i];
        if (index!= -1){
            // we have a student at this space
            // cout << "student detected at space " << i << endl;
            int temp_payoff = 1;
            if (i -1 >= 0 && closest_student_dist[row_num][i-1] > 0){
                temp_payoff += closest_student_dist[row_num][i-1] -1;
            }
            if (i+1 < col_count &&closest_student_dist[row_num][i+1] > 0){
                temp_payoff += closest_student_dist[row_num][i+1] -1;
            }

            // Changed to cap at the maximum utlity
            students[index]->payoff = std::min(temp_payoff, students[index]->max_utility);

        } else {
            // cout << "no student detected at space " << i << endl;
        }
    }
}



// ITERATED BEST RESPONSE CODE BELOW ########################################################################################################################################
int Classroom::getUnhappiestStudent(){
    // This function returns the ID of the minimum payoff student
    int min_student = -1;
    int min_payoff = max_utility + 1; 
    
    for (int i = 0; i < students.size(); i++){
        if (students[i]->payoff < min_payoff){
            // we have a new unhappiest student
            min_student = i;
            min_payoff = students[i]->payoff;
        }
    }
    return min_student;
}

void Classroom::placeStudent(int ID, pair<int, int> loc){
    // This function places student with ID at loc.first row and loc.second col (only use after removing them)
    if (students[ID]->sitting){cout << "cannot place a student that is seating" << endl; throw exception();}
    // if (print_mode){cout << "moving student " << ID << " to row " << loc.first << " and col " << loc.second << endl;}
    students[ID]->row = loc.first;
    students[ID]->col = loc.second;
    students[ID]->sitting = true;
    layout[loc.first][loc.second].push_back(students[ID]);

}

void Classroom::removeStudent(int ID){
    // This function removes student with ID from the layout (and recalculates distances) (you may remove a student that is already removed)
    // if (!students[ID]->sitting){cout << "cannot remove a student that is not seating" << endl; throw exception();}

    // DOES NOT RECALCULATE PAYOFFS
    // first, we get the location of that student
    int row = students[ID]->row;
    int col = students[ID]->col;
    // now, we look at that cell on layout, and continue until we find that student
    
    int index_to_remove = find(layout[row][col].begin(), layout[row][col].end(), students[ID]) - layout[row][col].begin();
    layout[row][col].erase(layout[row][col].begin() + index_to_remove);
    reCalcDistances(row);
    reCalcPayoffs(row);
    students[ID]->sitting = false;
}

void Classroom::moveStudent(int ID){
    // This function will move a student with the input ID to a (presumably better) spot. 
    // Will choose the best available spot

    removeStudent(ID);

    // now we need to find the highest spot on the closest_student_distances 
    int best_row = 0;
    int best_col = 0;
    int best_payoff = 0;

    // we loop through every seat in the room, looking at closest student dist, and ensuring that it is empty
    for (int i = 0; i < row_count; i ++){
        for (int j = 0; j < col_count; j++){
            // we are at a specific seat, we only do anything if we have not already found a maximal seat
            if (best_payoff < max_utility){
                if (best_payoff < closest_student_dist[i][j] && layout[i][j].size() == 0){
                    // we have found an empty seat (i, j) that is better
                    best_row = i;
                    best_col = j;
                    best_payoff = closest_student_dist[i][j];
                }
            } 
        }
    }
    // now that we have the best seat, we sit the student there.
    placeStudent(ID, make_pair(best_row, best_col));
    // now we recalculate the distances on that row
    reCalcDistances(best_row);
    reCalcPayoffs(best_row);
}

bool Classroom::canImprove(int ID){
    // This function determines if the student with ID can improve their payoff by moving
    int old_row = students[ID]->row;
    int old_col = students[ID]->col;
    int old_payoff = students[ID]->payoff;
    // we do a test move of that student, and see if the payoff has increased, then move them back
    moveStudent(ID);
    // we see if the payoff has increased. 
    bool toReturn = (students[ID]->payoff > old_payoff);
    // now we replace the student in the original position
    // remove them from prospective new position
    removeStudent(ID);
    // put them back in the old position
    placeStudent(ID, make_pair(old_row, old_col));
    reCalcDistances(old_row);
    reCalcPayoffs(old_row);
    return toReturn;
}


bool Classroom::bestResponse(){
    // this function runs one iteration of the best response, and returns a boolean that is true if someone changed their spot, and false otherwise.
    vector<int> unhappiestStudentIDs;
    int least_happy_ID = getUnhappiestStudent();
    int smallest_payoff = students[least_happy_ID]->payoff;
    bool someoneImproved = false;
    // we need to offer all the unhappiest students an opportunity to change, and if they do, we return (those with the same payoff as smallest payoff)
    // loop through all students
    for (int i = 0; i < students.size(); i++){
        if(students[i]->payoff == smallest_payoff){
            // we have a least happy student, we offer them a change
            if (canImprove(i)){
                // student i can improve, so we move them and return
                moveStudent(i);
                if (print_mode){cout << "BEST RESPONSE: MOVED STUDENT: " << i << endl;}
                return true;
            }
            // no else statement: if that student can't move we ignore them
        }
        // no else statement: if that student doesn't have min payoff, we ignore them
    }
    // none of the lowest students moved, so we need to check all the students to see if there are any edge moves
    for (int i = 0; i < students.size(); i++){
        if(students[i]->payoff == smallest_payoff){
            // we have a least happy student, we ignore them (since we already got them)
            
        } else {
            if (canImprove(i)){
                // student i can improve, so we move them and return
                moveStudent(i);
                if(print_mode){cout << "BEST RESPONSE (EDGE MOVE): MOVED STUDENT: " << i << endl;}
                return true;
            }
        }
        // no else statement: if that student doesn't have min payoff, we ignore them
    }
    return false;
}

int Classroom::iteratedBestResponse(){ // changed to return how many times bestResponse moved a student instead of void
    // This function runs iterations of bestResponse until there is no valid move for a student
    if (print_mode) {cout << "STARTING BEST RESPONSE ##############################################################" << endl;}
    int i = 0;

    while(bestResponse()){
        i++;
        if (print_mode){
            printClassroom();
            printStudents();
        }
    }
    if (print_mode) {cout << "BEST RESPONSE TERMINATED\n\n\n\n"<< endl;}

    return i; 
}


void Classroom::moveStudents(map<int, pair<int, int>> newPositions){
    // this function takes in a map for the new positions (values) of some students (keys), and removes them from their current position, and sits them
    // iterate through each student
    for (auto i : newPositions){
        // if (print_mode){cout << "moving student " << i.first << " to row " <<}
        // remove that student
        removeStudent(i.first);
        // place that student at the new position
        placeStudent(i.first, i.second);
    }
}

set<pair<int, int>> Classroom::getEmptySeats(){
    // this function returns a set of all the currently empty seats
    set<pair<int, int>> empties;
    for (int i = 0 ; i < row_count; i++){
        for (int j = 0; j < col_count ; j++){
            // see if this seat is empty
            if (layout[i][j].size() == 0){
                empties.insert(make_pair(i, j));
            }
        }
    }
    if (print_mode){
        for (auto x: empties){
            // cout << "row " << x.first << " column " << x.second << " is empty" << endl;
        }
    }
    return empties;
}

void generateCombinations(vector<pair<int, int>>::iterator current, 
                          vector<pair<int, int>>::iterator end,
                          int x, 
                          set<pair<int, int>>& currentCombination, 
                          set<set<pair<int, int>>>& result) {
    // This function is a helper function for getCombinationsOf for recursion
    // Base case: If the current combination size is equal to x, add it to the result
    if (currentCombination.size() == x) {
        result.insert(currentCombination);
        return;
    }
    
    // Stop if we've exhausted the input set
    if (current == end) {
        return;
    }

    // Include the current pair in the combination
    currentCombination.insert(*current);
    generateCombinations(next(current), end, x, currentCombination, result);

    // Exclude the current pair and proceed to the next
    currentCombination.erase(*current);
    generateCombinations(next(current), end, x, currentCombination, result);
}




set<set<pair<int, int>>> Classroom::getCombinationsOf(set<pair<int, int>> empties, int number){
    // this function returns all combinations of number pairs in empties
    set<set<pair<int, int>>> result;

    // Edge case: If x is greater than the size of the input set, return an empty result
    if (number > empties.size() || number <= 0) {
        return result;
    }

    // Convert the input set to a vector for easier iteration
    vector<pair<int, int>> pairs(empties.begin(), empties.end());
    
    // Temporary set to hold the current combination
    set<pair<int, int>> currentCombination;

    // Start generating combinations
    generateCombinations(pairs.begin(), pairs.end(), number, currentCombination, result);

    // if (print_mode){
    //     for (const auto& combination : result) {
    //         cout << "{ ";
    //         for (const auto& pair : combination) {
    //             cout << "(" << pair.first << ", " << pair.second << ") ";
    //         }
    //         cout << "}\n";
    //     }
    // }
    
    
    return result;
}

map<int, pair<int, int>> Classroom::makeReseatMap(set<int> IDs, set<pair<int, int>> locations){
    // This function takes in a set of IDs and a set of locations, and outputs a map that can be used to reseat those students to those locations
    // does not consider order of students, seats a not neccessarily random permutation
    map<int, pair<int, int>> reseatMap;
    set<pair<int, int>> combination_copy = locations;
    // go through each ID and add it and one of the seats to the map
    for (int oneID: IDs){
        reseatMap[oneID] = *combination_copy.begin();
        combination_copy.erase(*combination_copy.begin());
    }
    return reseatMap;
}





pair<set<set<pair<int, int>>>, int> Classroom::getBestReseating(set<int> IDs){
    // we get all of the positions map(ID, LOCATION)
    map<int, pair<int, int>> oldPositions;

    // we first remove all of the students in the coalition from the board
    for (int oneID : IDs){
        oldPositions[oneID] = make_pair(students[oneID]->row, students[oneID]->col);
        removeStudent(oneID);
    }
    // now we have a board empty except for those students
    // get all the empty seats
    set<pair<int, int>> empties = getEmptySeats();
    // get all the combinations of empty seats for the number of players in the coalition
    set<set<pair<int, int>>> emptyCombs = getCombinationsOf(empties, IDs.size());

    // first we re-seat players into thier previous seats (they aren't sitting atm)
    for (auto i : oldPositions){
        placeStudent(i.first, i.second);
    }

    // integer to track the total payoff of the players between rounds
    int max_payoff = 0;
    // integer to track the current payoff of a combination
    int current_payoff = 0;
    //integer to track the highest paid member
    int max_individual_payoff = 0;
    // integer to track the lowest paid member
    int min_individual_payoff = 0;
    // integer to track the difference 
    int min_max_diff = 3;

    // set of set of positions that is all the maximal payoff combinations
    set<set<pair<int, int>>> bestCombinations;
    // set of rows to update the distances and payoffs of
    set<int> rowsToUpdate;

    // now we must re-seat the players to a new combination, calculate the total payoff of those players, and see which combination has the highest
    // for each empty combination, we calculate
    for(set<pair<int, int>> combination: emptyCombs){
        // create the map from player IDs to the elements of combination
        map<int, pair<int, int>> reseatMap;
        set<pair<int, int>> combination_copy = combination;
        // go through each ID and add it and one of the empty seats to the map
        for (int oneID: IDs){
            reseatMap[oneID] = *combination_copy.begin();
            rowsToUpdate.insert((*combination_copy.begin()).first);
            combination_copy.erase(*combination_copy.begin());

        }

        // reseat players in that map
        moveStudents(reseatMap);
        // if (print_mode){cout << "moved students to empty combination"<< endl; printClassroom();}

        // calculate the payoffs

        // go through each relevant row and update it
        for (int i : rowsToUpdate){
            reCalcDistances(i);
            reCalcPayoffs(i);        
        } 
        // compare the total of the payoffs between iterations of for loop. 
        current_payoff = 0;
        max_individual_payoff = 0;
        min_individual_payoff = max_utility;

        for (int oneID : IDs){
            // cout << "in this combination, person " << oneID << " gets payoff " << students[oneID]->payoff << endl;
            current_payoff += students[oneID]->payoff;
            max_individual_payoff = max(max_individual_payoff, students[oneID]->payoff);
            min_individual_payoff = min(min_individual_payoff, students[oneID]->payoff);
        }
        // check to see if this is a winning combination
        if (current_payoff > max_payoff){
            // flush all of the worse combinations
            bestCombinations.clear();
            // add the current combination
            bestCombinations.insert(combination);
            // update the max
            max_payoff = current_payoff;
            min_max_diff = max_individual_payoff - min_individual_payoff;
        
        } else if (current_payoff == max_payoff){
            // add this combination to the list
            if (max_individual_payoff - min_individual_payoff < min_max_diff){
                // we have a more fair solution that has the same total payoff, we prefer this
                            // flush all of the worse combinations
                bestCombinations.clear();
                // add the current combination
                bestCombinations.insert(combination);
                // update the max
                max_payoff = current_payoff;
                min_max_diff = max_individual_payoff - min_individual_payoff;
            } else if (max_individual_payoff - min_individual_payoff == min_max_diff){
                // we have an equally fair solution. we add it to the list
                bestCombinations.insert(combination);
            } 
            // if neither of those hit, then there is a equal payoff solution that is less equitable than the current, so we ignore it.
        }
    }

    // now we move the students back to their old positions
    moveStudents(oldPositions);
    return make_pair(bestCombinations, max_payoff);
}



Coalition Classroom::createCoalition(std::set<int> IDs){
    Coalition c;
    get<0>(c) = IDs;
    map<set<pair<int, int>>, map<int, double>> repositions;
    map<int, double> finalPayoffMap;

    // cout << "classroom before get best reseating--------------------------------------------------------------------" << endl;
    // printClassroom();
    pair<set<set<pair<int, int>>>, int> temp = getBestReseating(IDs);
    // cout << "classroom after get best reseating--------------------------------------------------------------------" << endl;
    // printClassroom();
    set<set<pair<int, int>>> bestReseating = temp.first;
    int total_coalition_payoff = temp.second;
    double individual_coalition_payoff = (double) total_coalition_payoff / (double) IDs.size();
    if (print_mode){cout << "each coalition member will get a payoff of: " << individual_coalition_payoff << endl;}

    if (print_mode){cout << "finished getting best seats. printing orignal classroom " << endl; printClassroom();}

    // classroom has remained the same

    if (print_mode){
        cout << "the best positions for reseating Students ";
        for (int id: IDs){
            cout << id << " ";
        }
        cout << " are: " << endl;
        for (const auto& combination : bestReseating) {
            cout << "{ ";
            for (const auto& pair : combination) {
                cout << "(" << pair.first << ", " << pair.second << ") ";
            }
            cout << "}\n";
        }
    }
    // for each combination in bestReseating
        // reseat members of coalition to those spots,
        // calculate the payoffs
        // make a map of ID -> payoff for each student
        // replace payoff of each coalition member with average of coalition members payoff


    // save the positions of the students in question
    // we get all of the positions map(ID, LOCATION)
    map<int, pair<int, int>> oldPositions;
    for (int oneID : IDs){
        oldPositions[oneID] = make_pair(students[oneID]->row, students[oneID]->col);
    }

    // for each combination in bestReseating
    for (set<pair<int, int>> combination: bestReseating){
        // reseat members of coalition to those spots
        moveStudents(makeReseatMap(IDs, combination));

        // calculate the payoffs
        for (int i = 0; i < row_count; i ++){
            reCalcDistances(i);
            reCalcPayoffs(i);
        }
        // make a map of ID -> payoff for each student
        map<int, double> payoffMap;
        for (auto student : students){
            payoffMap[student->ID] = (double) student->payoff;
        }
        // replace payoff of each coalition member with average of coalition members payoff
        for (int coalitionMember : IDs){
            payoffMap[coalitionMember] = individual_coalition_payoff;
        }

        if (print_mode){
            cout << "Contributing Payoff map:\n";
            for (const auto& pair : payoffMap) {
                cout << "ID: " << pair.first << ", Payoff: " << pair.second << "\n";
            }
        }
        repositions[combination] = payoffMap;


    }
    // move students to original seats
    moveStudents(oldPositions);
    // students will not move from this point onward
    get<1>(c)= repositions;

    // set finalpayoff map to 0
    for (int i = 0; i < students.size(); i++){
        finalPayoffMap[i] = 0;
    }
    // add each divided by the number of contributing repositionings

    for (auto reposition : repositions){
        for (int i = 0; i < students.size(); i++){
            // add the contributing payoff to the final payoff map
            finalPayoffMap[i] += reposition.second[i] / repositions.size();
        }
    }

    get<2>(c) = finalPayoffMap;

    if (print_mode){
        cout << "Final Coalition Payoff Map" << endl;
        for (auto student : finalPayoffMap){
            cout << "Student " << student.first << " may receive payoff of " << student.second << endl;
        }
    }


    return c;
}






// PRINTING CODE BELOW #################################################################################################################3
void Classroom::printDistances(){
    for (int i = 0; i < closest_student_dist.size(); i++){
        for (int j = 0; j < closest_student_dist[i].size(); j++){
            // cout << "__0" << closest_student_dist[i][j] << "__"; 
            // cout << "    ";
            // if (closest_student_dist[i][j] < 10){
            //     cout << "0" << closest_student_dist[i][j];
            // } else {
            //     cout << closest_student_dist[i][j];
            // }
            // cout << "    ";
        }
        // cout << "\n";
    }
}

void Classroom::whoCanImprove(){
    cout << "Printing List of Students: \n";
    for (int i = 0; i < students.size(); i++){
        students[i]->printStudent();
        if (canImprove(i)){
            cout << "above can improve"<< endl;
        } else {
            cout << "above cannot improve" << endl;
        }
    }
    cout << endl;
}




void Classroom::printClassroom(){
    if (rows.empty()){
        // cout << "CANNOT PRINT AN EMPTY CLASSROOM \n";
        return;
    }
    cout << "PRINTING CLASSROOM----------------------------------------- \n";
    cout << "SEATS\n";


    for (int i = 0; i < layout.size(); i++){
        for (int j = 0; j < layout[i].size(); j++){
            // now we are printing a specific seat

            for (int k = 0; k < layout[i][j].size(); k++){
                cout << layout[i][j][k]->ID << ",";
            }
            for (int k = layout[i][j].size(); k < 3; k++){
                cout << "__";
            }
            cout << "    ";

        }
        cout << endl;
    }

}

void Classroom::printStudents(){
    cout << "Printing List of Students: \n";
    for (int i = 0; i < students.size(); i++){
        students[i]->printStudent();
    }
    cout << endl;
}




// Utility function to print the partitions
void printPartitions(const set<set<set<int>>>& partitions) {
    int count = 1;
    cout<< "printing partition";
    for (const auto& partition : partitions) {
        cout << "Partition " << count++ << ": { ";
        for (const auto& coalition : partition) {
            cout << "{ ";
            for (int elem : coalition) {
                cout << elem << " ";
            }
            cout << "} ";
        }
        cout << "}\n";
    }
}

void printPartition(const set<set<int>>& partition) {
        cout <<  "{ ";
        for (const auto& coalition : partition) {
            cout << "{ ";
            for (int elem : coalition) {
                cout << elem << " ";
            }
            cout << "} ";
        }
        cout << "}\n";

}

void printCoalition(const set<int>& coalition) {
    cout << "{ ";
    for (int elem : coalition) {
        cout << elem << " ";
    }
    cout << "} ";
}



// Function to convert vector<vector<int>> to set<set<int>>
set<set<int>> convertToSetOfSets(const vector<vector<int>>& vecOfVecs) {
    set<set<int>> result;

    for (const auto& vec : vecOfVecs) {
        // Convert each vector<int> to set<int> and insert into the result
        result.insert(set<int>(vec.begin(), vec.end()));
    }

    return result;
}

 
// Function to generate all partitions
void Partition(vector<int> sete, int index,
               vector<vector<int> >& ans, set<set<set<int>>>& finalPartition)
{
 
    // If we have considered all elements
    // in the set print the partition
    if (index == sete.size()) {
        // printPartition(ans);
        finalPartition.insert(convertToSetOfSets(ans));
        return;
    }
 
    // For each subset in the partition
    // add the current element to it
    // and recall
    for (int i = 0; i < ans.size(); i++) {
        ans[i].push_back(sete[index]);
        Partition(sete, index + 1, ans, finalPartition);
        ans[i].pop_back();
    }
 
    // Add the current element as a
    // singleton subset and recall
    ans.push_back({ sete[index] });
    Partition(sete, index + 1, ans, finalPartition);
    ans.pop_back();
}
 
// Function to generate all
// partitions for a given set
set<set<set<int>>> allPartitions(vector<int> sete)
{
    vector<vector<int> > v;
    set<set<set<int>>> theFinal;
    Partition(sete, 0, v, theFinal);
    return theFinal;
}
 




void Classroom::createPartitions(){
    // this function populates the round with the partitions;
    // TODO: delete information properly to avoid memory leaks
    round.coalitionMap.clear();
    round.Partitions.clear();

    vector<int> everyPlayer(fullness);
    for (int i = 0; i < fullness; i++) {
        everyPlayer[i] = i;
    }

    set<set<set<int>>> allSetPartitions = allPartitions(everyPlayer);

    for (auto partition : allSetPartitions){
        // we are now considering a single partition 
        set<Coalition> partitionConstructorInput;

        // we check to see that each coalition is in the map so its calculations can be formed.
        for (auto coalition : partition){
            if (round.coalitionMap.find(coalition) == round.coalitionMap.end()){
                // we are missing a coalition, we compute it and add it
                addToCoalitionMap(coalition);
            }
            partitionConstructorInput.insert(round.coalitionMap[coalition]);
        }

        // now each of the coalitions is gauranteed to be formed, so we get all of them and then put them into the partition constructor
        Partition* p = new Partition(partitionConstructorInput);
        p->pID = round.Partitions.size();
        round.Partitions.push_back(p);
        round.partitionMap[partition] = p;
    }

}
void Classroom::addToCoalitionMap(set<int> IDs){
    // This function creates the coalition with the IDs, and adds it to the coalition map so that it can be used to create partitions.
    Coalition c = createCoalition(IDs);
    round.coalitionMap[IDs] = c;
}


void Classroom::printPartitions(){
    int i = 0;
    for (Partition* p : round.Partitions){
        cout << "Partition " << i << " :" << endl;
        p->printPartition();
        p->printAdjacencies();
        i++;
    }
    cout << "there are " << round.coalitionMap.size() << " many coalitions in our set" << endl;
}

void Classroom::fillAllAdjacencies(){
    // this function fills the adjacencies of every partition in the round
    // first we clear them in case there are strange nonempty ones

    for (Partition* p : round.Partitions){
        p->adjacencies.clear();
        fillAdjacencies(p);
    }
}



void Classroom::fillAdjacencies(Partition* p){
    // this function fills the adjacencies of one partition in the round
    // need to consider every coalition as the defectors
    // cout << "filling adjacencies for partition: " << endl;
    // p->printParition();
    // printPartition(p->coalition_IDs);
    // cout << round.coalitionMap.size() << " many iterations for each coalition" << endl;
    for(auto map_element : round.coalitionMap){
        set<int> coalition = map_element.first;
        // cout << "considering the coalition";
        // printCoalition(coalition);
        // cout << endl;
        // We only do this if we have reached a different partition, by using a coalition that is not in the partition
        if (p->coalition_IDs.count(coalition) == 0){
            // cout << "did not find the coalition wihtin partition, so analyzing it further" << endl;
            // coalition is the set of defectors, p->coalition_Ids is the partition
            set<set<int>> newPartitionIDs = breakPartition(p->coalition_IDs, coalition);
            // cout << "this coalition gives partition" ;
            // printPartition(newPartitionIDs);
            // now we get the partition pointer from the map in round
            Partition* q = round.partitionMap[newPartitionIDs];
            // cout << "that partition has map ";
            // q->printPartition();
            // now, we add q to the Partition's adjacencies with the proper key, which is the coalition itself. 
            p->adjacencies[coalition] = q;
        }
    }
}

void Classroom::thinAllAdjacencies(){
    for (Partition* p : round.Partitions){
        p->strong_adjacents.clear();
        p->weak_adjacents.clear();
        thinAdjacencies(p);
    }
}

void Classroom::thinAdjacencies(Partition* p){
    // cout << "thinning adjacencies on partition" << endl;
    // p->printPartition();
    // cout << p->adjacencies.size();
    for (auto map_element : p->adjacencies){
        set<int> coalition = map_element.first;
        Partition* q = map_element.second;
        // cout << "considering partition: " << endl;
        // p->printPartition();
        // cout << "against partition: "<< endl;
        // q->printPartition();



        // we want to consider the IDs in the coalition, and see if they do actually prefer q over p, if they all do prefer it, it is in the strong adjacencies, some prefer, and some don't, then it is in the weak adjacencies, and if everyone is indifferent, then they are not adjacent.
        bool strong = true;
        bool weak = true;
        bool indifferent = true;
        // cout << "begginning iteration through a coalition" << endl;
        for (int i : coalition){
            // cout << "considering student " << i << endl;
            // cout << "i may get " << p->studentExpectedPayoffs[i] << "under current partition, or " << q->studentExpectedPayoffs[i] << "with the coalition: " << endl;
            // printCoalition(coalition);
            if (q->studentExpectedPayoffs[i] < p->studentExpectedPayoffs[i]){
                // we have someone in the coalition who prefers the current coalition to the other choice.
                strong = false;
                weak = false;
                indifferent = false;
            } else if (q->studentExpectedPayoffs[i] == p->studentExpectedPayoffs[i]){
                // this current person is indifferent, so the adjacency is not strong
                strong = false;

            } else {
                // this current person does actually prefer q over p, so strong and weak remain true, indifferent is false;
                indifferent = false;
            }
        } 
        if (strong){
            p->strong_adjacents.insert(q);
        }
        if (weak & !indifferent){
            p->weak_adjacents.insert(q);
        }
    }
}





set<set<int>> Classroom::breakPartition(set<set<int>> OrigPartition, set<int> defectors){
    set<set<int>> newPartition;
    for(auto coalition : OrigPartition){
        // we consider each coalition in original partition, 
        // remove all of the defectors
        for (int defector : defectors){
            coalition.erase(defector);
        }
        // now, if this coalition has non-zero size, we add it to the new partition
        if (coalition.size() >0){
            newPartition.insert(coalition);
        }
    }

    newPartition.insert(defectors);
    return newPartition;

}



void Classroom::printBasicPartitions(){
    cout << "Partition List" << endl;
    for (int i = 0; i < round.Partitions.size(); i++){
        cout << "Partition " << i << ": ";
        printPartition(round.Partitions[i]->coalition_IDs);
        // cout << round.Partitions[i] ->pID;
        // cout << endl;
    }
}

void Classroom::printStrongAdjacency(){
    printBasicPartitions();
    cout << "Strong Adjacency List" << endl;
    for (int i = 0; i < round.Partitions.size(); i++){
        cout << "Partition " << round.Partitions[i]->pID << " has a directed edge to: ";
        for (Partition* p : round.Partitions[i]->strong_adjacents){
            cout << p->pID << " ";
        }
        cout << endl;
    }
}

void Classroom::printWeakAdjacency(){
    printBasicPartitions();
    cout << "Weak Adjacency List" << endl;
    for (int i = 0; i < round.Partitions.size(); i++){
        cout << "Partition " << round.Partitions[i]->pID << " has a directed edge to: ";
        for (Partition* p : round.Partitions[i]->weak_adjacents){
            cout << p->pID << " ";
        }
        cout << endl;
    }
}


















int main(){
    int choice;
    cout << "Options: " << endl;
    cout << "1. Aggregate test: N cases" << endl;
    cout << "2. Single test" << endl;
    cin >>  choice;

    if(choice == 1){

        int num_seats = 20;
        int num_rows = 1;
        double fullness = 3;
        int utility = 3;

        int iterations;

        // Take in seats, students, rows
        cout << "Utility = 3." << endl;
        cout << "Number of Seats: ";
        cin >> num_seats;
        cout << "Number of Students: ";
        cin >> fullness;
        cout << "Number of Rows: ";
        cin >> num_rows;
        cout << "Number of Iterations: ";
        cin >> iterations;

        double average_utility = 0.0;
        double average;

        double standard_deviation;

        vector<int> seat_changes;
        double avg_number_of_moves;

        vector<vector<int>> utilities; // each entry in this is the utility of all students from an individual game. utilities[0][2] is the count/number of students in game #0 with utility 2

        for(int i = 0; i < iterations; i++){

            Classroom room(num_seats, num_rows, fullness, utility, false);
            // room.printStudents();
            room.sitAllStudents(false);
            seat_changes.push_back(room.iteratedBestResponse());
            avg_number_of_moves += seat_changes[i];

            utilities.push_back(room.getStudentUtilities());
        }

        vector<double> deviations;

        // utilities now contains all student utilities for all games

        for(vector<int> game : utilities){
            // calculate stdev first, then other stuff
            double total_players = 0;
            double summed_squared_deviation = 0;
            double std_dev;

            average = 0;
            double number_of_players = accumulate(game.begin(), game.end(), 0.0);
            int total_utility = game[1] + game[2] * 2 + game[3] * 3;
            average = total_utility / number_of_players;
            average_utility += average;


            for(int i = 0; i < 4; i++){                    
                for(int j = game[i]; j > 0; j--){
                    double ind_deviation = i - average;
                    double squared_deviation = pow(ind_deviation, 2);
                    summed_squared_deviation += squared_deviation;
                    total_players++;
                }
            }
            std_dev = sqrt(summed_squared_deviation / (total_players - 1));
            deviations.push_back(std_dev);
        }

        for(int i = 0; i < utilities.size(); i++){
            standard_deviation += deviations[i];
        }

        average_utility /= iterations;
        avg_number_of_moves /= iterations;

        Classroom nash(num_seats, num_rows, fullness, utility, false);
        nash.sitAllStudentsNash();
        int best_nash_turns = nash.iteratedBestResponse();
        vector<int> nash_utility = nash.getStudentUtilities();
        double total_nash_util = 1 * nash_utility[1] + 2 * nash_utility[2] + 3 * nash_utility[3];
        double nash_average = total_nash_util / fullness;
        double optimal_nash_std_dev;
        
        double nash_summed_squared_deviation;

        nash.printClassroom();

        for(int i = 0; i < 4; i++){                    
            for(int j = nash_utility[i]; j > 0; j--){
                double ind_deviation = i - nash_average;
                double nash_squared_deviation = pow(ind_deviation, 2); 
                nash_summed_squared_deviation += nash_squared_deviation;
            }
        }
        optimal_nash_std_dev = sqrt(nash_summed_squared_deviation / (fullness - 1));
        cout << optimal_nash_std_dev << endl;

        cout << "RESULTS --------------------------" << endl;
        cout << "Approximately optimal nash took " << best_nash_turns << " times to reach a Nash" << endl;
        cout << "With average utility of " << average_utility << endl << endl;
        
        cout << "Aggregate random seeding: " << endl;
        cout << "Number of Iterations: " << iterations << endl;
        cout << "Average Utility: " << average_utility << endl;
        cout << "Average Number of Moves: " << avg_number_of_moves << endl;
        cout << "Standard Deviation of Player Utilities: " << standard_deviation / iterations << endl;
        cout << "Standard Deviation of Optimal Nash Player Utilities: " << optimal_nash_std_dev << endl;


    }

    else if(choice == 2){

        bool test_list = false;
        if (test_list) {

        parammap test_params = {
            /*{Index, {seats, rows, fullness, util, optional} }*/
            {0, {40,2,3,3, 1}},
            /*{1,{40,2,3,3, 2}},
            {2, {40, 2, 10, 3}},
            {3,{10,1,3,3}},
            {4,{10,1,3,3}},
            {5,{5,1,3,3}},*/
        };

        runTests(test_params,
        exampletestlist);

        } else {

            // Variables for the user to fill
            int input_students;
            int input_seats;
            int input_rows;
            int input_maxutil;


            // Take in seats, students, rows
            cout << "Number of Seats: ";
            cin >> input_seats;
            cout << "Number of Students: ";
            cin >>input_students;
            cout << "Number of Rows: ";
            cin >>input_rows;

            // Take in a maximum utility
            cout << "Chose maximum utility for all students: ";
            cin >> input_maxutil;
            if (input_maxutil < 1)
                throw invalid_argument("Must be a positive number");

            // Create classroom and seat students
            Classroom room(input_seats, input_rows, input_students, input_maxutil, false);
            cout << "Created Classroom. Seating all students" << endl;
            room.sitAllStudents(true);
            // room.iteratedBestResponse();
            map<int, pair<int, int>> PPexample;
            // PPexample[0] = make_pair(0, 0);
            // PPexample[1] = make_pair(0, 1);
            // PPexample[2] = make_pair(0, 2);
            // room.moveStudents(PPexample);







            // room.getCombinationsOf(room.getEmptySeats(), 3);
            // set<int> testCoalition;
            // testCoalition.insert(0);
            // testCoalition.insert(1);
            // testCoalition.insert(2);
            // set<int> testCoalition2;
            // testCoalition2.insert(3);
            // testCoalition2.insert(4);

            // set<Coalition> coalitionSet;
            // // Coalition temp = room.createCoalition(testCoalition);
            // // coalitionSet.insert(temp);
            // cout << "making first coalition" << endl;
            // room.printClassroom();
            // coalitionSet.insert(room.createCoalition(testCoalition2));
            // cout << "finished making first coalition" << endl;
            // room.printClassroom();
            // cout << " making second coalition" << endl;
            // coalitionSet.insert(room.createCoalition(testCoalition));
            // cout << "finished making second coalition" << endl;
            // room.printClassroom();

            


            // cout << "making a partition" << endl<< endl<<endl;
            // Classroom::Partition myPartition = Classroom::Partition(coalitionSet);
            // cout << "printing the partition" << endl<< endl<<endl;
            // myPartition.printPartition();
            cout << " finished seating students, now creating partitions" << endl;

            room.createPartitions();
            cout << endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl;
            cout << "finished creating Partitions, now filling adjacencies" << endl;
            room.fillAllAdjacencies();
            cout << "finished creating adjacencies, now  printing them" << endl;
            // room.printPartitions();
            cout << "finished filling adjacencies, now thinning adjacencies" << endl;
            


            room.thinAllAdjacencies();
            // room.printBasicPartitions();
            room.printStrongAdjacency();
            room.printWeakAdjacency();
            cout << "finished excecution" << endl;
            room.printClassroom();

            // Input set
            // Input set
            // set<int> inputSet = {1, 2, 3, 4};

            // // Generate all partitions
            // set<set<set<int>>> all;

            // // Print the result
            // // cout << "All partitions of the set: { ";
            // // for (int elem : inputSet) cout << elem << " ";
            // // cout << "}\n";

            // // printPartitions(allPartitions);


            // // The size of the set
            // int n = 5;
        
            // // Initialize the set as
            // // {1, 2, ..., n}
            // vector<int> set(n);
            // for (int i = 0; i < n; i++) {
            //     set[i] = i + 1;
            // }
            // cout << "All partition of the set will be : " << endl;
        
            // // Generate all partitions of the set
            // all = allPartitions(set);
            // printPartitions(all);




            // cout << "finished" << endl;
            // map<int, pair<int, int>> newpos;
            // for (int i = 0; i < 5; i++){
            //     newpos[i] = make_pair(0,0);
            // }
            // room.moveStudents(newpos);
            // room.printClassroom();
            // room.getEmptySeats();

            // vector<Student> students = initStudents(num_seats / fullness , num_greedy);

            // Class.rows = fillrows(num_seats / fullness); // fills all students and calls display function within

        }
    }
    return 0;
}
