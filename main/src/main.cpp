#include "main.h"
#include <iostream>
#include <vector>
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
        throw invalid_argument("Seat count must be a multiple of row count");
    }
    col_count = seat_num/row_count;
    // initialize the grid of students in the classroom (-1 is empty, 0-N are used by student with that number)
    vector<int> temp_row;
    for (int i = 0; i < row_count; i++){
        temp_row.clear();
        for (int j = 0; j < col_count; j++){
            temp_row.push_back(-1);
        }
        rows.push_back(temp_row);
    }
    //TODO: include the initialization of students for the classroom, and the seating functionality
}

void Classroom::printClassroom(){
    if (rows.empty()){
        cout << "CANNOT PRINT AN EMPTY CLASSROOM \n";
        return;
    }
    cout << "PRINTING CLASSROOM----------------------------------------- \n";
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
    cout << "------------------------------------------------\n";
    
    
}




int main(){
    int test_num = 0;
    cout << "game theory" << std::endl;

    if (test_num == 0){
        //Doing test of classroom printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 50;

        Classroom room(num_seats, num_rows, fullness);
        room.printClassroom();
    }else {

        







        // vector<Student> students = initStudents(num_seats / fullness , num_greedy);

        // Class.rows = fillrows(num_seats / fullness); // fills all students and calls display function within

    }
    return 0;
    }
