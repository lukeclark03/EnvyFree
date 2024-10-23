# main.h

## Student Struct
### Variables
- bool greedy: Greediness 
- int preferred_row: Preferred Row
	- -1 is no preference
	- else it's the row number
- int ID: #Question *What does this do exactly?*
- bool sitting: Whether or not the student has been placed in a seat
- int payoff: What payoff they have so far, updated each round 
### Functions
- printStudent: Prints information on the student into the terminal
	- The ID of the student
	- Whether or not they have been placed (sitting)
	- // Whether or not they are greedy
	- What payoff do they currently have

# Classroom Struct
## Variables
- int seat_num : Number of seats
- int row_count: number of rows in the class
- int col_count: number of seats in each row
- int  fullness :
- int number_sat: Number of students that are sitting
- bool naive: 
- 2D array: rows
- 2D array: closest distance

# Main.cpp

## Main: 
- Running tests with numbers 0-6
- Otherwise, run program

## Function initStudents:
- Should initialize a vector of students;
	- *May initialize random personalities in the future?*

## Function fillRows
- *Should*