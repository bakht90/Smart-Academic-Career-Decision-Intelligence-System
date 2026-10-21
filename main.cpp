#include <iostream>

#include "include/Student.h"
#include "include/Academic.h"
#include "include/Career.h"
#include "include/Dashboard.h"

using namespace std;

int main()
{
    Student student;
    Academic academic;
    Career career;
    Dashboard dashboard;

    bool login = false;
    int choice;

    do
    {
        cout << "\n=========================================\n";
        cout << " SMART ACADEMIC & CAREER DECISION SYSTEM\n";
        cout << "=========================================\n";

        cout << "1. Student Management\n";
        cout << "2. Academic Management\n";
        cout << "3. Career Assessment\n";
        cout << "4. Dashboard\n";
        cout << "5. Exit\n";

        cout << "\nEnter Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: //student management
        {
            int c;

            cout << "\n1. Register\n";
            cout << "2. Login\n";
            cout << "3. View Profile\n";
            cout << "4. Update Academic Information\n";
            cout << "5. Back\n";

            cout << "Choice: ";
            cin >> c;

            if (c == 1)
            {
                student.registerStudent();
            }
            else if (c == 2)
            {
                login = student.loginStudent();

                if (login)
                {
                    academic.setUsername(
                        student.getUsername());

                    career.setUsername(
                        student.getUsername());
                }
            }
            else if (c == 3) //profile view
            {
                if (login)
                    student.viewProfile();
                else
                    cout << "Please login first.\n";
            }
            else if (c == 4)
            {
                if (login)
                    student.updateAcademicInfo();
                else
                    cout << "Please login first.\n";
            }

            break;
        }

        case 2: //academic management
        {
            if (!login)
            {
                cout << "Please login first.\n";
                break;
            }

            int c;

            cout << "\n1. Add Assignment\n";
            cout << "2. View Assignments\n";
            cout << "3. Mark Assignment Done\n";
            cout << "4. Calculate GPA\n";
            cout << "5. Exam Countdown\n";
            cout << "6. Back\n";

            cout << "Choice: ";
            cin >> c;

            if (c == 1)
                academic.addAssignment();

            else if (c == 2)
                academic.viewAssignments();

            else if (c == 3)
                academic.markAssignmentDone();

            else if (c == 4)
                academic.calculateGPA();

            else if (c == 5)
                academic.examCountdown();

            break;
        }

        case 3: //career 
        {
            if (!login)
            {
                cout << "Please login first.\n";
                break;
            }

            int c;

            cout << "\n1. Skill Assessment\n";
            cout << "2. Career Recommendation\n";
            cout << "3. Career Match Score\n";
            cout << "4. Back\n";

            cout << "Choice: ";
            cin >> c;

            if (c == 1)
                career.addSkill();

            else if (c == 2)
                career.careerRecommendation();

            else if (c == 3)
                career.careerMatchScore();

            break;
        }

        case 4:  //dashborad
        {
            if (login)  //Performance Analyzer
            {
                dashboard.performanceAnalyzer(
                    student.getAttendance(),
                    student.getGPA(),
                    academic.getPendingAssignments()
                );

                dashboard.smartAlerts(
                    student.getAttendance(),
                    student.getGPA(),
                    academic.getPendingAssignments()
                );

                dashboard.performanceLevel(
                    student.getAttendance(),
                    student.getGPA()
                );
            }
            else
            {
                cout << "Please login first.\n";
            }

            break;
        }

        case 5:
            cout << "\nThank You!\n";
            break;

        default:
            cout << "Invalid Choice!\n";
        }

    } while (choice != 5);

    return 0;
}