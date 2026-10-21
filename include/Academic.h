#ifndef ACADEMIC_H //header guard
#define ACADEMIC_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector> //use to store multiple valeus

using namespace std;

struct AssignmentRecord
{
    string id;
    string username;
    string name;
    string status;
};

class Academic
{
private:
    string assignmentName;
    int daysLeft;
    string currentUsername;

public:
    Academic();

    void setUsername(const string&);

    void addAssignment();
    void viewAssignments();
    void markAssignmentDone();
    int getPendingAssignments();
    void calculateGPA();
    void examCountdown();

    bool addAssignment(const string&, const string&); //username nd assignment
    vector<AssignmentRecord> getAssignments(const string&);
    bool setAssignmentStatus(const string&, int, const string&);
    int getPendingAssignments(const string&);
    float calculateGPA(float) const;
};

#endif
