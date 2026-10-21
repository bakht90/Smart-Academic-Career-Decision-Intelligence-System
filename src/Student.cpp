#include "../include/Student.h"

const string DATA_FILE = "data/users.txt";

Student::Student()
{
    username = "";
    password = "";
    name = "";
    age = 0;
    department = "";
    gpa = 0;
    attendance = 0;
}

bool Student::registerStudent(
    const string& studentName,
    int studentAge,
    const string& studentDepartment,
    const string& studentUsername,
    const string& studentPassword,
    float studentGpa,
    int studentAttendance)
{
    if (studentName.empty() ||
        studentDepartment.empty() ||
        studentUsername.empty() ||
        studentPassword.empty())
    {
        return false;
    }

    if (studentAge <= 0 ||
        studentAge > 120 ||
        studentGpa < 0 ||
        studentGpa > 4 ||
        studentAttendance < 0 ||
        studentAttendance > 100)
    {
        return false;
    }

    ifstream input(DATA_FILE); //file read

    string fileName;

    while (getline(input, fileName))
    {
        string age;
        string department;
        string user;
        string pass;
        string fileGpa;
        string att;

        getline(input, age);
        getline(input, department);
        getline(input, user);
        getline(input, pass);
        getline(input, fileGpa);
        getline(input, att);

        if (user == studentUsername)
            return false;
    }

    ofstream file(DATA_FILE, ios::app);  //data write ( append mode)

    if (!file)
        return false;

    file << studentName << "\n"
         << studentAge << "\n"
         << studentDepartment << "\n"
         << studentUsername << "\n"
         << studentPassword << "\n"
         << studentGpa << "\n"
         << studentAttendance << "\n";

    name = studentName;
    age = studentAge;
    department = studentDepartment;
    username = studentUsername;
    password = studentPassword;
    gpa = studentGpa;
    attendance = studentAttendance;

    return true;
}

bool Student::loginStudent(
    const string& studentUsername,
    const string& studentPassword)
{
    ifstream file(DATA_FILE);

    if (!file)
        return false;

    string fileName;

    while (getline(file, fileName))
    {
        string age;
        string department;
        string user;
        string pass;
        string fileGpa;
        string att;

        getline(file, age);
        getline(file, department);
        getline(file, user);
        getline(file, pass);
        getline(file, fileGpa);
        getline(file, att);

        if (studentUsername == user &&
            studentPassword == pass)
        {
            name = fileName;
            this->age = stoi(age); //string to integer
            this->department = department; //this refer to current object
            username = user;
            password = pass;
            gpa = stof(fileGpa); //string to float
            attendance = stoi(att);

            return true;
        }
    }

    return false;
}

bool Student::getStudent(    
    const string& studentUsername)
{
    return loginStudent(
        studentUsername,  //login function call
        password);
}

bool Student::updateAcademicInfo(
    float newGpa,
    int newAttendance)
{
    if (username.empty())
        return false;

    if (newGpa < 0 ||
        newGpa > 4 ||
        newAttendance < 0 ||
        newAttendance > 100)
    {
        return false;
    }

    ifstream input(DATA_FILE);

    if (!input) //file open or not
        return false;

    string line; //one line temporary store
    string updated;
    bool found = false;

    while (getline(input, line))
    {
        string age;
        string department;
        string user;
        string pass;
        string fileGpa;
        string att;

        getline(input, age);
        getline(input, department);
        getline(input, user);
        getline(input, pass);
        getline(input, fileGpa);
        getline(input, att);

        if (user == username)
        {
            fileGpa = to_string(newGpa);
            att = to_string(newAttendance);
            found = true;
        }

        updated += line + "\n";
        updated += age + "\n";
        updated += department + "\n";
        updated += user + "\n";
        updated += pass + "\n";
        updated += fileGpa + "\n";
        updated += att + "\n";
    }

    input.close();

    if (!found)
        return false;

    ofstream output(DATA_FILE, ios::trunc); //Purani file ka content clear karke updated data

    if (!output)
        return false;

    output << updated;

    gpa = newGpa;
    attendance = newAttendance;

    return true;
}

void Student::registerStudent()
{
    string n;
    string d;
    string u;
    string p;

    int a;
    int att;

    float g;

    cout << "\n===== Student Registration =====\n";

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, n);

    cout << "Enter Age: ";
    cin >> a;
    cin.ignore();

    cout << "Enter Department: ";
    getline(cin, d);

    cout << "Create Username: ";
    cin >> u;

    cout << "Create Password: ";
    cin >> p;

    cout << "Enter GPA (0.0 - 4.0): ";
    cin >> g;

    cout << "Enter Attendance (%): ";
    cin >> att;

    if (registerStudent(
            n,
            a,
            d,
            u,
            p,
            g,
            att))
    {
        cout << "\nRegistration Successful!\n";
    }
    else
    {
        cout << "\nRegistration failed. Check values or username.\n";
    }
}

bool Student::loginStudent()
{
    string u;
    string p;

    cout << "\n===== Student Login =====\n";

    cout << "Username: ";
    cin >> u;

    cout << "Password: ";
    cin >> p;

    bool result =
        loginStudent(u, p);

    if (result)
    {
        cout << "\nLogin Successful!\n";
        cout << "Welcome, " << name << "!\n";
    }
    else
    {
        cout << "\nInvalid Username or Password!\n";
    }

    return result;
}

void Student::viewProfile()
{
    cout << "\n===== Student Profile =====\n";

    cout << "Name: "
         << name
         << "\n";

    cout << "Age: "
         << age
         << "\n";

    cout << "Department: "
         << department
         << "\n";

    cout << "Username: "
         << username
         << "\n";

    cout << "GPA: "
         << gpa
         << "\n";

    cout << "Attendance: "
         << attendance
         << "%\n";
}

void Student::updateAcademicInfo()
{
    float newGpa;
    int newAttendance;

    cout << "\n===== Update Academic Information =====\n";

    cout << "Current GPA: "
         << gpa
         << "\n";

    cout << "Enter New GPA (0.0 - 4.0): ";
    cin >> newGpa;

    cout << "Current Attendance: "
         << attendance
         << "%\n";

    cout << "Enter New Attendance (%): ";
    cin >> newAttendance;

    if (updateAcademicInfo(
            newGpa,
            newAttendance))
    {
        cout << "\nAcademic information updated successfully!\n";
    }
    else
    {
        cout << "\nInvalid academic information.\n";
    }
}

float Student::getGPA()
{
    return gpa;
}

int Student::getAttendance()
{
    return attendance;
}

string Student::getUsername() const
{
    return username;
}

string Student::getName() const
{
    return name;
}

int Student::getAge() const
{
    return age;
}

string Student::getDepartment() const
{
    return department;
}

string Student::getPassword() const
{
    return password;
}