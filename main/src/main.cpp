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

    //TODO: include the seating functionality
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

    } else {
        // TODO: implement ungreedy functionality here
        throw invalid_argument("Ungreedy seating functionality has not yet been created");
    }


    number_sat++;
}

void Classroom::reCalcDistances(int row_num){

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

void Classroom::printStudents(){
    cout << "Printing List of Students: \n";
    for (int i = 0; i < students.size(); i++){
        students[i].printStudent();        
    }
}




int main(){
    int test_num = 3;

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
    }
    else{

        







        // vector<Student> students = initStudents(num_seats / fullness , num_greedy);

        // Class.rows = fillrows(num_seats / fullness); // fills all students and calls display function within

    }
    return 0;
    }
