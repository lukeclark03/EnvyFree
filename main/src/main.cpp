#include "main.h"
#include <iostream>
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


int main(){

    cout << "game theory" << std::endl;

    int num_seats = 40;
    int num_rows = 2;
    int fullness = 50;
    int num_greedy = 10;

    Classroom Class(num_seats, num_rows, fullness); 

    vector<Student> students = initStudents(num_seats / fullness , num_greedy);

    // Class.rows = fillrows(num_seats / fullness); // fills all students and calls display function within


    return 0;
    }
