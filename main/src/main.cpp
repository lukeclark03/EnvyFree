#include "main.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <iterator>
using namespace std;


vector<Student> initStudents(int num_students, int num_greedy){
    vector<Student> students;
    int greedy_added;

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


Classroom::Classroom(int seat_num_, int row_count_, int fullness_, bool naive_, int util_){

    // Initialization of member variables
    this->seat_num = seat_num_;
    this->row_count = row_count_;
    this->fullness = fullness_;
    this->naive = naive_;
    max_utility = util_;

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
        Student* new_student = new Student(naive, -1, i, false, -1, max_utility);  
        students.push_back(new_student);
    }
    printStudents();

}

void Classroom::sitAllStudents(bool debug){
    // this function seats all the students in the classroom, one at a time, with or without debugging
    // seed the random number generator
    srand((unsigned) time(NULL));

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
    printStudents();

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
    cout << "sitting a student: " << toSit->ID << endl;



    // get a random row
    int rand_row = rand() % row_count;
    // get a random column
    int rand_col = rand() % col_count;

    // cout << "sitting student randomly at row " << rand_row << "and col: " << rand_col << endl;
    // add the student to that spot
    toSit->printStudent();
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



    // // Previous implementation

    //  if (toSit.greedy){
    //     if (toSit.preferred_row == -1){
    //         // find the best row to sit in
    //         int best_row = -1;
    //         int best_dist = 0;
    //         for (int i = 0; i < row_count; i++){
    //             auto best_seat_i = max_element(closest_student_dist[i].begin(), closest_student_dist[i].end());
    //             if (*best_seat_i > best_dist){
    //                 best_row = i;
    //                 best_dist = *best_seat_i;
    //             }
    //         }
    //         // we now have the best row stored in best_row. Now we find the best seat in row best_row
    //         auto best_seat = max_element(closest_student_dist[best_row].begin(), closest_student_dist[best_row].end());
    //         int best_seat_index = distance(closest_student_dist[best_row].begin(), best_seat);
    //         rows[best_row][best_seat_index] = toSit.ID;
    //         cout << "Sat student " << toSit.ID << " at seat " << best_seat_index << " for prospective payoff: " << closest_student_dist[best_row][best_seat_index] -1<<  endl << endl;
    //         students[number_sat].sitting = true;
    //         reCalcDistances(best_row);
    //         reCalcPayoffs(best_row);
    //     } else {
    //         // TODO: implement preferred row functionality here
    //         throw invalid_argument("preffered row functionality has not yet been created");
    //     }


    // } else {
    //     if (toSit.preferred_row == -1){
    //         // find the best row to sit in
    //         int best_row = -1;
    //         int best_dist = 0;
    //         for (int i = 0; i < row_count; i++){
    //             auto best_seat_i = max_element(closest_student_dist[i].begin(), closest_student_dist[i].end());
    //             if (*best_seat_i > best_dist){
    //                 best_row = i;
    //                 best_dist = *best_seat_i;
    //             }
    //         }
    //         // we now have the best row stored in best_row. Now we find the best seat in row best_row
    //         auto best_seat = max_element(closest_student_dist[best_row].begin(), closest_student_dist[best_row].end());
    //         int best_seat_index = distance(closest_student_dist[best_row].begin(), best_seat);
    //         // so now we know the first best seat index. We need to add an amount to it so that it is in the middle of the block. The block size is closest_student_dist[best_row][best_seat_index] - 1
    //         // so we need to add half of the block size
    //         best_seat_index += (closest_student_dist[best_row][best_seat_index] - 2)/2;
    //         rows[best_row][best_seat_index] = toSit.ID;
    //         cout << "Sat student " << toSit.ID << " at seat " << best_seat_index << " for prospective payoff: " << closest_student_dist[best_row][best_seat_index] -1<<  endl << endl;
    //         students[number_sat].sitting = true;
    //         reCalcDistances(best_row);
    //         reCalcPayoffs(best_row);
    //     }else {
    //         // TODO: implement preferred row functionality here
    //         throw invalid_argument("preffered row functionality has not yet been created");
    //     }


    //     // TODO: implement ungreedy functionality here
    // }
    
    
    // Find maximum possible payoff
    // Select a random one of the maxiumum payoff seats
    // Since payoffs are already capped at the max set by the user
    // There is no need to worry it will select a higher payoff than that on purpose.
    // std::random_device seed;
    // std::mt19937 randgen(seed());
    // std::uniform_int_distribution<uint32_t> randdist{0, static_cast<unsigned int>(rows_mapped_by_payoff.back().size())};

    // auto iter = rows_mapped_by_payoff.back().begin();
    // std::advance(iter, randdist(randgen));

    // coordinates seat = *iter;

    // rows[seat.first][seat.second] = toSit.ID;

    // cout << "Sat student " << toSit.ID << " at seat (" << seat.first << "," << seat.second << ") for distance from others: " << closest_student_dist[seat.first][seat.second] << 
    // " and payoff: " <<  min(closest_student_dist[seat.first][seat.second], toSit.max_utility) <<  endl << endl;
    // students[number_sat].sitting = true;



    // reCalcDistances(seat.first, toSit);
    // reCalcPayoffs(seat.first, toSit);

    // number_sat++;
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




    // // Previous logic wasn't working well.
    
    // // we will be working with rows[row_num] and closest_student_dist[row_num]
    // for (int i = 0; i < col_count; i++){
    //     // cout << "updating seat: " << i << endl;
    //     if (rows[row_num][i] != -1){
    //         // This seat is occupied, so it has a distance of 0
    //         closest_student_dist[row_num][i] = 0;
    //     } else {
    //         // we have an open seat (dist >=1)
    //         closest_student_dist[row_num][i] = 1;
    //         // now we increment it until we reach an impediment (student or end of row) on either side
    //         int l_dist = 1;
    //         int r_dist =1;
    //         while (i-l_dist >= 0 && rows[row_num][i - l_dist] == -1){
    //             // cout << "decrementing left" << endl;
    //             closest_student_dist[row_num][i]++;
    //             l_dist++;
    //         }

    //         while (i+r_dist < col_count && rows[row_num][i + r_dist] == -1){
    //             // cout << "incrementing right" << endl;
    //             closest_student_dist[row_num][i]++;
    //             r_dist++;
    //         }
    //     }
    // }



    
    
    
    // // Start at beginning of each row and scan until reaching a set of students (right now can only be 1)

    // // Edge case: Scan until first student and fill with distance only to that student
    // bool hit = false;
    // int s = 0;
    // while (rows[row_num][s] == -1) s++; /* for set, should be testing for NOT empty instead*/

    // for (int k = 0; k < s; k++)
    //     closest_student_dist[row_num][k] = s - k;

    // // Now begin scanning after first student; at i = s, should hit immediately.
    // bool nexthit = false;
    // for (int i = s; i < col_count;){
    //     while (!hit && i < col_count) {
    //         if (rows[row_num][i] != -1 /* for set, should be testing for empty instead*/) {
    //             hit = true;
    //             closest_student_dist[row_num][i] = 0;
    //             break;
    //         }
    //         i++;
    //     }

    //     // Scan for next student
    //     int j = i+1;
    //     while (!nexthit && j < col_count) {
    //         if (rows[row_num][j] != -1) {
    //             nexthit = true;
    //             closest_student_dist[row_num][j] = 0;
    //             break;
    //         }
    //         j++;
    //     }

    //     // Sweep middle and assign distances.
    //     // When j is the end, only assign k-i as distance, for edge case.
    //     for (int k = i + 1; k < j; k++) {
    //         if (nexthit)
    //             closest_student_dist[row_num][k] = min(k-i, j-k);
    //         else
    //             closest_student_dist[row_num][k] = k-i;
    //     }

    //     // Reset hit vars
    //     hit = nexthit = false;
    //     // set i to after j
    //     i = j + 1;
    // }

    /*
    Example: 1 _ 1 1 _
    Scan until hit student...
    hits student at i = 0, assigns 0
    scans i=1, finds nothing
    scans i=2, finds student, assigns 0
    last student set was i=0
    fill triangularly student /\ student
    this case should be 1 in i=1... etc.

    */

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
            /*
            // if (closest_student_dist[row_num][i] == 0){
            //     temp_payoff = 1;
            // }
            // if (i -1 >= 0 && closest_student_dist[row_num][i-1] > 0){
            //     temp_payoff = closest_student_dist[row_num][i-1];
            // }
            // if (i+1 < col_count &&closest_student_dist[row_num][i+1] > 0){
            //     temp_payoff = closest_student_dist[row_num][i+1];
            // }
            // cout << "setting payout to " << temp_payoff << endl;
            */
            // Changed to cap at the maximum utlity
            students[index]->payoff = std::min(temp_payoff, students[index]->max_utility);

            // Categorizing payoffs into rows_mapped_by_dist

            //TODO: Fix/move this step to instead modify payoff left (if exists) and payoff recieved as if from a person coming in's perspective.
            // Or find a way to make it per-person

            // Find if the updated payoff is in the correct category (if it updated or not). If it isn't, j != payoff.
            // erase and emplace it into the right one.
            for (int j = 0; j < rows_mapped_by_payoff.size(); j++) {
                set<coordinates>& current_row = rows_mapped_by_payoff[j];
                auto found = current_row.find(coordinates(row_num,i));
                if (found != current_row.end() && j != students[index]->payoff) {
                    coordinates savedpair = *found;
                    current_row.erase(found);
                    rows_mapped_by_payoff[students[index]->payoff].emplace(savedpair);
                    break;
                }
            }


        } else {
            // cout << "no student detected at space " << i << endl;
        }
    }
}

void Classroom::printDistances(){
    for (int i = 0; i < closest_student_dist.size(); i++){
        for (int j = 0; j < closest_student_dist[i].size(); j++){
            cout << "__0" << closest_student_dist[i][j] << "__"; 
            // cout << "    ";
            // if (closest_student_dist[i][j] < 10){
            //     cout << "0" << closest_student_dist[i][j];
            // } else {
            //     cout << closest_student_dist[i][j];
            // }
            cout << "    ";
        }
        cout << "\n";
    }
}

// ITERATED BEST RESPONSE CODE BELOW
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


void Classroom::removeStudent(int ID){
    // This function removes student with ID from the layout (and recalculates distances) 
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
    students[ID]->row = best_row;
    students[ID]->col = best_col;
    students[ID]->sitting = true;
    layout[best_row][best_col].push_back(students[ID]);
    // now we recalculate the distances on that row
    reCalcDistances(best_row);
    reCalcPayoffs(best_row);
}

bool Classroom::canImprove(int ID){
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
    students[ID]->row = old_row;
    students[ID]->col = old_col;
    students[ID]->sitting = true;
    layout[old_row][old_col].push_back(students[ID]);
    reCalcDistances(old_row);
    reCalcPayoffs(old_row);
    return toReturn;
}













void Classroom::whoCanImprove(){
    // cout << "Printing List of Students: \n";
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
        cout << "CANNOT PRINT AN EMPTY CLASSROOM \n";
        return;
    }
    cout << "PRINTING CLASSROOM----------------------------------------- \n";
    cout << "SEATS\n";
    // for (int i = 0; i < rows.size(); i++){
    //     for (int j = 0; j < rows[i].size(); j++){
    //         if (rows[i][j] == -1){
    //             cout << "__";
    //         } else if (rows[i][j] < 10){
    //             cout << "0" << rows[i][j];
    //         } else {
    //             cout << rows[i][j];
    //         }
    //         cout << "  ";
    //     }
    //     cout << "\n";
    // }

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







    // cout << "PAYOFFS" << endl;
    // // print payoffs
    // for (int i = 0; i < rows.size(); i++){
    //     for (int j = 0; j < rows[i].size(); j++){
    //         if (rows[i][j] == -1){
    //             cout << "__";
    //         } else if (students[rows[i][j]].payoff < 10){
    //             cout << "0" << students[rows[i][j]].payoff;
    //         } else {
    //             cout << students[rows[i][j]].payoff;
    //         }
    //         cout << "  ";
    //     }
    //     cout << "\n";
    // }
    cout << "DISTANCES" << endl;
    printDistances();
    cout << "\n";
    cout << "------------------------------------------------\n\n";
}

void Classroom::printStudents(){
    cout << "Printing List of Students: \n";
    for (int i = 0; i < students.size(); i++){
        students[i]->printStudent();
    }
    cout << endl;
}

int main(){
    int test_num = 5;

    if (test_num == 0){
        //Doing test of classroom printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 3;
        int utility = 3;

        Classroom room(num_seats, num_rows, fullness, true, utility);
        room.printClassroom();
    }else if (test_num ==1){
        //Doing test of single student printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 3;
        int utility = 3;

        Classroom room(num_seats, num_rows, fullness, true, utility);
        room.students[1]->printStudent();

    }else if (test_num ==2){
        //Doing test of student list printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 10;
        int utility = 3;

        Classroom room(num_seats, num_rows, fullness, true, utility);
        room.printStudents();
    }else if (test_num ==3){
        //Doing test of student distance printing functionality
        int num_seats = 10;
        int num_rows = 1;
        int fullness = 3;
        int utility = 3;

        Classroom room(num_seats, num_rows, fullness, true, utility);
        room.printDistances();
    } else if (test_num ==4){
        //Doing test of sitStudent functionality
        int num_seats = 10;
        int num_rows = 1;
        int fullness = 3;
        int utility = 3;

        Classroom room(num_seats, num_rows, fullness, true, utility);
        // room.printStudents();
        room.printClassroom();
        room.sitStudent();
        room.printClassroom();
        room.sitStudent();
        room.printClassroom();
        room.printStudents();

    }  else if (test_num ==5){
        //Doing test of sitAllStudents functionality
        int num_seats = 20;
        int num_rows = 2;
        int fullness = 14;
        int utility = 3;

        Classroom room(num_seats, num_rows, fullness, true, utility);
        // room.printStudents();
        room.sitAllStudents(true);
        room.whoCanImprove();
        room.printClassroom();

        // cout << "unhappiest student: " << endl;
        // room.students[room.getUnhappiestStudent()]->printStudent();
        // cout << "moving that student" << endl;
        // room.moveStudent(room.getUnhappiestStudent());
        // // room.removeStudent(room.getUnhappiestStudent());
        // room.printClassroom();
        // room.printStudents();

    }
    else{

        // Variables for the user to fill
        int input_students;
        int input_seats;
        int input_rows;
        int input_maxutil;
        int choice;
        bool choice_bool;

        // Take in seats, students, rows
        cout << "Number of Seats: ";
        cin >> input_seats;
        cout << "Number of Students: ";
        cin >>input_students;
        cout << "Number of Rows: ";
        cin >>input_rows;


        cout << "Creating Classroom with  " << input_seats << " seats and " << input_students << " students" << endl;
        cout << "Choose strategy: \n1. Naive Greedy\n2. Greedy\n";

        // Chose strategy (Is this still relevant right now?)
        cin>>choice;
        if (choice == 1){
            choice_bool = true;
            cout << "You chose 1.Naive Greedy" << endl;
        } else if (choice == 2){
            choice_bool = false;
            cout << "You chose 2. Greedy" << endl;
        } else {
            throw invalid_argument("Must choose either 1 or 2.");
        }

        // Take in a maximum utility
        cout << "Chose maximum utility for all students: ";
        cin >> input_maxutil;
        if (input_maxutil < 1)
            throw invalid_argument("Must be a positive number");

        // Create classroom and seat students
        Classroom room(input_seats, input_rows, input_students, choice_bool, input_maxutil);
        cout << "Created Classroom. Seating all students" << endl;
        room.sitAllStudents(true);

        // vector<Student> students = initStudents(num_seats / fullness , num_greedy);

        // Class.rows = fillrows(num_seats / fullness); // fills all students and calls display function within

    }
    return 0;
    }
