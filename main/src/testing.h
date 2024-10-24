#include "main.h"
#include <functional>
#include <iostream>
#include <vector>
#include <map>
#include <optional>

/* Explanation of testing framework

Run any number of tests by writing a test function or in main,
then list them all in a vector of function pointers, vectFuncts, like in the example below.

Then, provide a map of lists of parameters, so you can run any number of tests by their index in the vector
this can be list initialized, so easy initialization like in the example below:

example: 

    runTests({
        {1,{40,2,3,3, 2}},
    },

    will run test with index 1,
    40 seats,
    2 rows,
    fullness of 3,
    max utility of 3,
    and optional value of 2.

*/

 struct parameterlist {
    int seats; 
    int rows; 
    int fullness; 
    int util;
    int optional = -1;
 };

 using vectFuncts = std::vector<std::function<void(parameterlist)>>;

 const std::function<Classroom(parameterlist)> setup = 
    [](parameterlist par) 
    {
        return Classroom(
        par.seats, 
        par.rows, 
        par.fullness, 
        par.util
        );
    };


void testClassPrint(parameterlist par) {
    setup(par).printClassroom();
}

void testStudentPrint(parameterlist par) {
    int s = par.optional == -1 ? 1 : par.optional;
    setup(par).students[s]->printStudent();
}

void testStudentListPrint(parameterlist par) {
    setup(par).printStudents();
}

void testDistancePrint(parameterlist par) {
    setup(par).printDistances();
}

void testSitStudent(parameterlist par) {
    int opt = par.optional == -1 ? 3 : par.optional;
    auto temp = setup(par);
    for (int times = 0; times < 3; times++) {
        temp.printClassroom();
        temp.sitStudent();
    }
    temp.printClassroom();
    temp.printStudents();
}

void testSitAllStudents(parameterlist par) {
    auto temp = setup(par);
    temp.sitAllStudents(true);
    temp.iteratedBestResponse();
}

vectFuncts exampletestlist = {
    testClassPrint,
    testStudentPrint,
    testStudentListPrint,
    testDistancePrint,
    testSitStudent,
    testSitAllStudents    
};


void runTests(const std::map<int, parameterlist>& num_parameters, vectFuncts testList) {
    int i = 0;
    auto test = [&]() {
        return (num_parameters.count(i) == 1);
    };

    for_each(testList.begin(),testList.end(), [&]( const std::function<void(parameterlist)>& func) {
        if (test()) func(num_parameters.at(i++));
    });
}