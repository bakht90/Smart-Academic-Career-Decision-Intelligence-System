#include "../include/Dashboard.h"

int Dashboard::performanceScore(
    int attendance,
    float gpa,
    int assignments) const
{
    double score = (gpa / 4) * 50;

    score += (attendance / 100.0) * 40;

    if (assignments <= 0)
        score += 10;
    else if (assignments == 1)
        score += 8;
    else if (assignments == 2)
        score += 6;
    else if (assignments == 3)
        score += 4;
    else
        score += 2;

    if (score > 100)
        score = 100;

    if (score < 0)
        score = 0;

    return (int)(score + 0.5);
}

string Dashboard::performanceStatus(
    int attendance,
    float gpa,
    int assignments) const
{
    if (attendance >= 85 &&
        gpa >= 3.5 &&
        assignments <= 1)
    {
        return "Excellent Academic Performance";
    }

    if (attendance >= 75 &&
        gpa >= 2.5)
    {
        return "Average Academic Performance";
    }

    return "Poor Academic Performance";
}

string Dashboard::performanceLevelText(
    int attendance,
    float gpa) const
{
    if (attendance >= 85 &&
        gpa >= 3.5)
    {
        return "Excellent Student";
    }

    if (attendance >= 75 &&
        gpa >= 2.5)
    {
        return "Average Student";
    }

    return "At Risk Student";
}

vector<string> Dashboard::alertMessages(
    int attendance,
    float gpa,
    int assignments) const
{
    vector<string> alerts;

    if (attendance < 75)
        alerts.push_back("Improve Attendance.");

    if (gpa < 2.5)
        alerts.push_back("Improve GPA.");

    if (assignments > 3)
        alerts.push_back("Complete Pending Assignments.");

    if (alerts.empty())
        alerts.push_back("No Alerts.");

    return alerts;
}

void Dashboard::performanceAnalyzer(
    int attendance,
    float gpa,
    int assignments)
{
    cout << "\n========== Academic Performance ==========\n";

    cout << "Attendance: "
         << attendance
         << "%\n";

    cout << "GPA: "
         << gpa
         << "\n";

    cout << "Pending Assignments: "
         << assignments
         << "\n";

    cout << "\n"
         << performanceStatus(
                attendance,
                gpa,
                assignments)
         << "\n";
}

void Dashboard::smartAlerts(
    int attendance,
    float gpa,
    int assignments)
{
    cout << "\n========== Smart Alerts ==========\n";

    vector<string> alerts =
        alertMessages(
            attendance,
            gpa,
            assignments);

    for (string alert : alerts)
        cout << "- " << alert << "\n";
}

void Dashboard::performanceLevel(
    int attendance,
    float gpa)
{
    cout << "\n========== Performance Level ==========\n";

    cout << performanceLevelText(
        attendance,
        gpa)
         << "\n";
}

