#include "../include/Academic.h"
#include <sstream>

const string DATA_FILE = "data/assignments.txt";

Academic::Academic()
{
    assignmentName = "";
    daysLeft = 0;
    currentUsername = "";
}

void Academic::setUsername(const string& username)
{
    currentUsername = username;
}

bool Academic::addAssignment(
    const string& username,
    const string& name)
{
    if (username.empty() ||
        name.empty() ||
        name.find('|') != string::npos)
    {
        return false;
    }

    ofstream file(DATA_FILE, ios::app); //append

    if (!file)
        return false;

    file << username << "|" << name << "|Pending\n";

    return true;
}

vector<AssignmentRecord> Academic::getAssignments(
    const string& username)
{
    vector<AssignmentRecord> result; //empty list..assignmnet store

    ifstream file(DATA_FILE); //read file

    if (!file)
        return result;

    string line;
    int id = 0;

    while (getline(file, line))
    {
        vector<string> parts;
        string part;

        stringstream ss(line); //stringstream line ko pieces mein divide karne mein help karta hai

        while (getline(ss, part, '|'))
            parts.push_back(part); //vector end..item add

        if (parts.size() == 3)
        {
            if (parts[0] != username)
                continue;

            AssignmentRecord a;

            a.id = to_string(id++);
            a.username = parts[0];
            a.name = parts[1];
            a.status = parts[2];

            result.push_back(a); //list mei add
        }
        else if (parts.size() == 2 && !username.empty())
        {
            AssignmentRecord a;

            a.id = to_string(id++);
            a.username = username;
            a.name = parts[0];
            a.status = parts[1];

            result.push_back(a);
        }
    }

    return result;
}

bool Academic::setAssignmentStatus(
    const string& username,
    int index,
    const string& status)
{
    if (status != "Pending" &&
        status != "Completed")
    {
        return false;
    }

    ifstream input(DATA_FILE);

    if (!input)
        return false;

    vector<string> lines;
    string line;

    while (getline(input, line))
        lines.push_back(line);

    input.close();

    int visible = 0;
    bool changed = false;

    for (string& current : lines)
    {
        vector<string> parts;
        string part;

        stringstream ss(current);

        while (getline(ss, part, '|'))
            parts.push_back(part);

        bool belongs = false;

        if (parts.size() == 3)
            belongs = parts[0] == username;
        else if (parts.size() == 2)
            belongs = true;

        if (!belongs)
            continue;

        if (visible == index)
        {
            if (parts.size() == 3)
                current = parts[0] + "|" + parts[1] + "|" + status;
            else
                current = parts[0] + "|" + status;

            changed = true;
            break;
        }

        visible++;
    }

    if (!changed)
        return false;

    ofstream output(DATA_FILE, ios::trunc);

    if (!output)
        return false;

    for (string line : lines)
        output << line << '\n';

    return true;
}

int Academic::getPendingAssignments(
    const string& username)
{
    int count = 0;

    vector<AssignmentRecord> list =
        getAssignments(username);

    for (AssignmentRecord a : list)
    {
        if (a.status == "Pending")
            count++;
    }

    return count;
}

float Academic::calculateGPA(float percentage) const
{
    if (percentage < 0 || percentage > 100)
        return -1;

    return (percentage / 100) * 4;
}

void Academic::addAssignment()
{
    if (currentUsername.empty())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\nEnter Assignment Name: ";

    cin.ignore();
    getline(cin, assignmentName);

    if (addAssignment(currentUsername, assignmentName))
        cout << "\nAssignment Added Successfully!\n";
    else
        cout << "\nUnable to add assignment.\n";
}

void Academic::viewAssignments()
{
    cout << "\n========== Assignments ==========\n";

    vector<AssignmentRecord> list =
        getAssignments(currentUsername);

    if (list.empty())
    {
        cout << "No assignments found.\n";
        return;
    }

    for (int i = 0; i < list.size(); i++)
    {
        cout << i + 1 << ". "
             << list[i].name
             << " [" << list[i].status << "]\n";
    }
}

void Academic::markAssignmentDone()
{
    vector<AssignmentRecord> list =
        getAssignments(currentUsername);

    if (list.empty())
    {
        cout << "\nNo assignments found.\n";
        return;
    }

    viewAssignments();

    int choice;

    cout << "\nEnter Assignment Number to Mark as Done: ";
    cin >> choice;

    if (choice < 1 || choice > list.size())
    {
        cout << "\nInvalid Assignment Number!\n";
        return;
    }

    if (setAssignmentStatus(
            currentUsername,
            choice - 1,
            "Completed"))
    {
        cout << "\nAssignment marked as Completed!\n";
    }
    else
    {
        cout << "\nUnable to update assignment.\n";
    }
}

int Academic::getPendingAssignments()
{
    return getPendingAssignments(currentUsername);
}

void Academic::calculateGPA()
{
    float percentage;

    cout << "\nEnter Percentage: ";
    cin >> percentage;

    float gpa = calculateGPA(percentage);

    if (gpa < 0)
        cout << "\nInvalid Percentage!\n";
    else
        cout << "\nCalculated GPA: " << gpa << '\n';
}

void Academic::examCountdown()
{
    cout << "\nDays Remaining in Exam: ";
    cin >> daysLeft;

    if (daysLeft < 0)
        cout << "\nInvalid Number of Days!\n";
    else
        cout << "\nOnly " << daysLeft
             << " day(s) left for exams.\n";
}