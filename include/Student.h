#ifndef STUDENT_H  //if not defined
#define STUDENT_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Student
{
private:
    string username, password, name, department;
    int age, attendance;
    float gpa;

public:
    Student();

    // Console functions
    void registerStudent();
    bool loginStudent();
    void viewProfile();
    void updateAcademicInfo();

    // Data functions
    bool registerStudent(
        const string&,
        int,
        const string&,
        const string&,
        const string&,
        float,
        int
    );

    bool loginStudent( //username nd password
        const string&,  
        const string&
    );

    bool updateAcademicInfo(
        float,
        int
    );

    bool getStudent(
        const string& 
    );

    // Getters
    float getGPA();
    int getAttendance();

    string getUsername() const;
    string getName() const;
    int getAge() const;
    string getDepartment() const;
    string getPassword() const;
};

#endif