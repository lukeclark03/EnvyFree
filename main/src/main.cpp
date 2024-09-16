#include "main.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


vector<Student> initStudents(int num_students, int num_greedy){
    vector<Student> students;
    int greedy_added;

    for(int i = 0; i < num_students; i++){

        

    }

    return students;
}

void fillrows(int num_students){
    // put a student into a row
    


    // call display function to portray all rows with new student
}


Classroom::Classroom(int seat_num_, int row_count_, int fullness_){
    this->seat_num = seat_num_;
    this->row_count = row_count_;
    this->fullness = fullness_;
    if (seat_num % row_count != 0){
        throw invalid_argument("Seat count must be a multiple of row count.");
    } else if (fullness > seat_num){
        throw invalid_argument("Must have fewer students than seats.");
    }
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

    // initialize the grid of nearest distances in the classroom 
    for (int i = 0; i < row_count; i++){
        temp_row.clear();
        for (int j = 0; j < col_count; j++){
            temp_row.push_back(min(j+1,col_count-j));
        }
        closest_student_dist.push_back(temp_row);
    }


    // All students are greedy at the moment
    for (int i = 0; i < fullness; i++){
        // Adding students to the vector of students
        // at this point, all students are greedy (true) and none of them have a preferred row (-1)
        students.push_back(Student(true, -1, i, false));
    }

    
}

void Classroom::sitStudent(){
    // This function seats a single student, the next in the list
    if (number_sat >= fullness){
        throw invalid_argument("Cannot sit another student, everyone has been sat");
    }
    if (students[number_sat].sitting){
        throw invalid_argument("Cannot sit a student that is already seated");
    }
    Student toSit = students[number_sat];
    if (toSit.greedy){
        // TODO: implement greedy functionality here
        if (toSit.preferred_row == -1){
            // find the best row to sit in
            int best_row = -1;
            int best_dist = 0;
            for (int i = 0; i < row_count; i++){
                auto best_seat_i = max_element(closest_student_dist[i].begin(), closest_student_dist[i].end());
                if (*best_seat_i > best_dist){
                    best_row = i;
                }
            }
            // we now have the best row stored in best_row. Now we find the best seat in row best_row
            auto best_seat = max_element(closest_student_dist[best_row].begin(), closest_student_dist[best_row].end());
            int best_seat_index = distance(closest_student_dist[best_row].begin(), best_seat);
            rows[best_row][best_seat_index] = toSit.ID;
            students[number_sat].sitting = true;
            reCalcDistances(best_row);
        }else {
            // TODO: implement preferred row functionality here
            throw invalid_argument("preffered row functionality has not yet been created");
        }


    } else {
        // TODO: implement ungreedy functionality here
        throw invalid_argument("Ungreedy seating functionality has not yet been created");
    }


    number_sat++;
}

void Classroom::reCalcDistances(int row_num){
    // we will be working with rows[row_num] and closest_student_dist[row_num]
    for (int i = 0; i < col_count; i++){
        if (rows[row_num][i] != -1){
            // This seat is occupied, so it has a distance of 0
            closest_student_dist[row_num][i] = 0;
        } else {
            // we have an open seat (dist >=1)
            closest_student_dist[row_num][i] = 1;
            // now we increment it until we reach an impediment (student or end of row) on either side
            int dist = 1;
            while (i+ dist < col_count && i - dist > 0 && rows[row_num][i - dist] == -1 && rows[row_num][i+ dist] == -1){
                closest_student_dist[row_num][i] ++;
                dist++; 
            }
        }
    }
}

void Classroom::printDistances(){
    for (int i = 0; i < closest_student_dist.size(); i++){
        for (int j = 0; j < closest_student_dist[i].size(); j++){
            if (closest_student_dist[i][j] < 10){
                cout << "0" << closest_student_dist[i][j];
            } else {
                cout << closest_student_dist[i][j];
            }
            cout << "  ";
        }
        cout << "\n";
    }
}

void Classroom::printClassroom(){
    if (rows.empty()){
        cout << "CANNOT PRINT AN EMPTY CLASSROOM \n";
        return;
    }
    cout << "PRINTING CLASSROOM----------------------------------------- \n";
    cout << "SEATS\n";
    for (int i = 0; i < rows.size(); i++){
        for (int j = 0; j < rows[i].size(); j++){
            if (rows[i][j] == -1){
                cout << "__";
            } else if (rows[i][j] < 10){
                cout << "0" << rows[i][j];
            } else {
                cout << rows[i][j];
            }
            cout << "  ";
        }
        cout << "\n";
    }
    printDistances();
    cout << "\n";
    cout << "------------------------------------------------\n";
}

void Classroom::printStudents(){
    cout << "Printing List of Students: \n";
    for (int i = 0; i < students.size(); i++){
        students[i].printStudent();        
    }
}




int main(){
    int test_num = 4;

    if (test_num == 0){
        //Doing test of classroom printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness);
        room.printClassroom();
    }else if (test_num ==1){
        //Doing test of single student printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness);
        room.students[1].printStudent();
    
    }else if (test_num ==2){
        //Doing test of student list printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 10;

        Classroom room(num_seats, num_rows, fullness);
        room.printStudents();
    }else if (test_num ==3){
        //Doing test of student distance printing functionality
        int num_seats = 10;
        int num_rows = 1;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness);
        room.printDistances();
    } else if (test_num ==4){
        //Doing test of student sitting functionality
        int num_seats = 10;
        int num_rows = 1;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness);
        // room.printStudents();
        room.printClassroom();
        room.sitStudent();
        room.printClassroom();
        room.sitStudent();
        room.printClassroom();
        room.printStudents();
        
    }
    else{

        







        // vector<Student> students = initStudents(num_seats / fullness , num_greedy);

        // Class.rows = fillrows(num_seats / fullness); // fills all students and calls display function within

    }
    return 0;
    }
