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


Classroom::Classroom(int seat_num_, int row_count_, int fullness_, bool naive_){
    this->seat_num = seat_num_;
    this->row_count = row_count_;
    this->fullness = fullness_;
    this->naive = naive_;
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
            temp_row.push_back(col_count+1);
        }
        closest_student_dist.push_back(temp_row);
    }


    // Students are greedy/nongreedy depending on the class variable
    for (int i = 0; i < fullness; i++){
        // Adding students to the vector of students
        // at this point, all students are greedy (true) and none of them have a preferred row (-1)
        students.push_back(Student(naive, -1, i, false, -1));
    }

    
}

void Classroom::sitAllStudents(bool debug){
    // this function seats all the students in the classroom, one at a time, with or without debugging
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
    if (number_sat >= fullness){
        throw invalid_argument("Cannot sit another student, everyone has been sat");
    }
    if (students[number_sat].sitting){
        throw invalid_argument("Cannot sit a student that is already seated");
    }
    Student toSit = students[number_sat];
    if (toSit.greedy){
        if (toSit.preferred_row == -1){
            // find the best row to sit in
            int best_row = -1;
            int best_dist = 0;
            for (int i = 0; i < row_count; i++){
                auto best_seat_i = max_element(closest_student_dist[i].begin(), closest_student_dist[i].end());
                if (*best_seat_i > best_dist){
                    best_row = i;
                    best_dist = *best_seat_i;
                }
            }
            // we now have the best row stored in best_row. Now we find the best seat in row best_row
            auto best_seat = max_element(closest_student_dist[best_row].begin(), closest_student_dist[best_row].end());
            int best_seat_index = distance(closest_student_dist[best_row].begin(), best_seat);
            rows[best_row][best_seat_index] = toSit.ID;
            cout << "Sat student " << toSit.ID << " at seat " << best_seat_index << " for prospective payoff: " << closest_student_dist[best_row][best_seat_index] -1<<  endl << endl;
            students[number_sat].sitting = true;
            reCalcDistances(best_row);
            reCalcPayoffs(best_row);
        }else {
            // TODO: implement preferred row functionality here
            throw invalid_argument("preffered row functionality has not yet been created");
        }


    } else {
        if (toSit.preferred_row == -1){
            // find the best row to sit in
            int best_row = -1;
            int best_dist = 0;
            for (int i = 0; i < row_count; i++){
                auto best_seat_i = max_element(closest_student_dist[i].begin(), closest_student_dist[i].end());
                if (*best_seat_i > best_dist){
                    best_row = i;
                    best_dist = *best_seat_i;
                }
            }
            // we now have the best row stored in best_row. Now we find the best seat in row best_row
            auto best_seat = max_element(closest_student_dist[best_row].begin(), closest_student_dist[best_row].end());
            int best_seat_index = distance(closest_student_dist[best_row].begin(), best_seat);
            // so now we know the first best seat index. We need to add an amount to it so that it is in the middle of the block. The block size is closest_student_dist[best_row][best_seat_index] - 1
            // so we need to add half of the block size
            best_seat_index += (closest_student_dist[best_row][best_seat_index] - 2)/2;
            rows[best_row][best_seat_index] = toSit.ID;
            cout << "Sat student " << toSit.ID << " at seat " << best_seat_index << " for prospective payoff: " << closest_student_dist[best_row][best_seat_index] -1<<  endl << endl;
            students[number_sat].sitting = true;
            reCalcDistances(best_row);
            reCalcPayoffs(best_row);
        }else {
            // TODO: implement preferred row functionality here
            throw invalid_argument("preffered row functionality has not yet been created");
        }
        
        
        // TODO: implement ungreedy functionality here
    }


    number_sat++;
}

void Classroom::reCalcDistances(int row_num){
    // we will be working with rows[row_num] and closest_student_dist[row_num]
    for (int i = 0; i < col_count; i++){
        // cout << "updating seat: " << i << endl;
        if (rows[row_num][i] != -1){
            // This seat is occupied, so it has a distance of 0
            closest_student_dist[row_num][i] = 0;
        } else {
            // we have an open seat (dist >=1)
            closest_student_dist[row_num][i] = 2;
            // now we increment it until we reach an impediment (student or end of row) on either side
            int l_dist = 1;
            int r_dist =1;
            while (i-l_dist >= 0 && rows[row_num][i - l_dist] == -1){
                // cout << "decrementing left" << endl;
                closest_student_dist[row_num][i]++;
                l_dist++;
            }
            
            while (i+r_dist < col_count && rows[row_num][i + r_dist] == -1){
                // cout << "incrementing right" << endl;
                closest_student_dist[row_num][i]++;
                r_dist++;
            }
        }
    }
}

void Classroom::reCalcPayoffs(int row_num){
    // we will be working with rows[row_num] and closest_student_dist[row_num]
    // cout << "recalculating payoffs" << endl;
    for (int i = 0; i < col_count; i++){

        if (rows[row_num][i]!= -1){
            // we have a student at this space
            // cout << "student detected at space " << i << endl;
            int temp_payoff;
            temp_payoff = 1;
            if (i -1 >= 0 && closest_student_dist[row_num][i-1] > 0){
                temp_payoff += closest_student_dist[row_num][i-1] -1;
            }
            if (i+1 < col_count &&closest_student_dist[row_num][i+1] > 0){
                temp_payoff += closest_student_dist[row_num][i+1] -1;
            }
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
            students[rows[row_num][i]].payoff = temp_payoff;

        } else {
            // cout << "no student detected at space " << i << endl;
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
    cout << "PAYOFFS" << endl;
    // print payoffs
    for (int i = 0; i < rows.size(); i++){
        for (int j = 0; j < rows[i].size(); j++){
            if (rows[i][j] == -1){
                cout << "__";
            } else if (students[rows[i][j]].payoff < 10){
                cout << "0" << students[rows[i][j]].payoff;
            } else {
                cout << students[rows[i][j]].payoff;
            }
            cout << "  ";
        }
        cout << "\n";
    }
    // printDistances();
    cout << "\n";
    cout << "------------------------------------------------\n\n";
}

void Classroom::printStudents(){
    cout << "Printing List of Students: \n";
    for (int i = 0; i < students.size(); i++){
        students[i].printStudent();        
    }
    cout << endl;
}




int main(){
    int test_num = 6;

    if (test_num == 0){
        //Doing test of classroom printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness, true);
        room.printClassroom();
    }else if (test_num ==1){
        //Doing test of single student printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness, true);
        room.students[1].printStudent();
    
    }else if (test_num ==2){
        //Doing test of student list printing functionality
        int num_seats = 40;
        int num_rows = 2;
        int fullness = 10;

        Classroom room(num_seats, num_rows, fullness, true);
        room.printStudents();
    }else if (test_num ==3){
        //Doing test of student distance printing functionality
        int num_seats = 10;
        int num_rows = 1;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness, true);
        room.printDistances();
    } else if (test_num ==4){
        //Doing test of sitStudent functionality
        int num_seats = 10;
        int num_rows = 1;
        int fullness = 3;

        Classroom room(num_seats, num_rows, fullness, true);
        // room.printStudents();
        room.printClassroom();
        room.sitStudent();
        room.printClassroom();
        room.sitStudent();
        room.printClassroom();
        room.printStudents();
        
    }  else if (test_num ==5){
        //Doing test of sitAllStudents functionality
        int num_seats = 10;
        int num_rows = 1;
        int fullness = 6;

        Classroom room(num_seats, num_rows, fullness, true);
        // room.printStudents();
        room.sitAllStudents(true);
    }
    else{

        int input_students;
        int input_seats;
        int input_rows;
        int choice;
        bool choice_bool;
        cout << "Number of Seats: ";
        cin >> input_seats;
        cout << "Number of Students: ";
        cin >>input_students;
        cout << "Number of Rows: ";
        cin >>input_rows;
        
        // cout << input_students << endl;
        // cout << input_seats << endl;
        cout << "Creating Classroom with  " << input_seats << " seats and " << input_students << " students" << endl;
        cout << "Choose strategy: \n1. Naive Greedy\n2. Greedy\n";
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
        Classroom room(input_seats, input_rows, input_students, choice_bool);
        cout << "Created Classroom. Seating all students" << endl;
        room.sitAllStudents(true);






        // vector<Student> students = initStudents(num_seats / fullness , num_greedy);

        // Class.rows = fillrows(num_seats / fullness); // fills all students and calls display function within

    }
    return 0;
    }
