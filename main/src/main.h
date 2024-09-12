#include<vector>

struct Student {

    bool greedy;
    int preferred_row;
    int ID;

    Student(int greedy, int preferred_row, int ID) : greedy(greedy), preferred_row(preferred_row), ID(ID) {};

};

struct Classroom {

    int seat_num;
    int row_count;
    int fullness;
    std::vector<std::vector<int>> rows;
    std::vector<Student> students;

    Classroom(int seat_num, int row_count, int fullness) : seat_num(seat_num), row_count(row_count), fullness(fullness) {};

};
