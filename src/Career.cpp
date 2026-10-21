#include "../include/Career.h"
#include <vector>

const string DATA_FILE = "data/skills.txt";

Career::Career()
{
    currentUsername = "";
    skill = "";
    career = "";
    matchScore = 0;
}

void Career::setUsername(const string& username)
{
    currentUsername = username;
}

string Career::getCareer(
    const string& selectedSkill) const
{
    if (selectedSkill == "C++")
        return "Software Developer";

    if (selectedSkill == "HTML CSS JavaScript")
        return "Frontend Web Developer";

    if (selectedSkill == "WordPress")
        return "WordPress Developer";

    if (selectedSkill == "Python")
        return "Python Developer";

    if (selectedSkill == "Database")
        return "Database Administrator";

    if (selectedSkill == "Networking")
        return "Network Administrator";

    if (selectedSkill == "Cyber Security")
        return "Cyber Security Analyst";

    if (selectedSkill == "UI/UX Design")
        return "UI/UX Designer";

    return "General IT Professional";
}

int Career::getMatchScore(
    const string& selectedCareer) const
{
    if (selectedCareer == "Software Developer")
        return 95;

    if (selectedCareer == "Frontend Web Developer")
        return 92;

    if (selectedCareer == "WordPress Developer")
        return 90;

    if (selectedCareer == "Python Developer")
        return 94;

    if (selectedCareer == "Database Administrator")
        return 88;

    if (selectedCareer == "Network Administrator")
        return 87;

    if (selectedCareer == "Cyber Security Analyst")
        return 96;

    if (selectedCareer == "UI/UX Designer")
        return 89;

    return 70;
}

bool Career::setSkill(
    const string& username,
    const string& selectedSkill)
{
    string validSkills[] =
    {
        "C++",
        "HTML CSS JavaScript",
        "WordPress",
        "Python",
        "Database",
        "Networking",
        "Cyber Security",
        "UI/UX Design"
    };

    bool valid = false;

    for (string s : validSkills)
    {
        if (s == selectedSkill)
            valid = true;
    }

    if (!valid || username.empty())
        return false;

    ifstream input(DATA_FILE);

    vector<string> lines;
    string line;

    while (getline(input, line))
        lines.push_back(line);

    input.close();

    bool updated = false;

    for (string& current : lines)
    {
        int position = current.find('|');

        if (position != string::npos)
        {
            string savedUser =
                current.substr(0, position);

            if (savedUser == username)
            {
                current =
                    username + "|" + selectedSkill;

                updated = true;
                break;
            }
        }
    }

    if (!updated)
        lines.push_back(
            username + "|" + selectedSkill
        );

    ofstream output(DATA_FILE, ios::trunc);

    if (!output)
        return false;

    for (string line : lines)
    {
        if (!line.empty())
            output << line << '\n';
    }

    skill = selectedSkill;
    career = getCareer(skill);
    matchScore = getMatchScore(career);

    return true;
}

string Career::getSkill(
    const string& username)
{
    ifstream file(DATA_FILE);

    string line;

    while (getline(file, line))
    {
        int position = line.find('|');

        if (position != string::npos)
        {
            string savedUser =
                line.substr(0, position);

            if (savedUser == username)
            {
                skill =
                    line.substr(position + 1);

                return skill;
            }
        }
    }

    return "";
}

void Career::addSkill()
{
    int choice;

    cout << "\n========== Skill Assessment ==========\n";
    cout << "1. C++\n";
    cout << "2. HTML, CSS & JavaScript\n";
    cout << "3. WordPress\n";
    cout << "4. Python\n";
    cout << "5. Database (SQL)\n";
    cout << "6. Networking\n";
    cout << "7. Cyber Security\n";
    cout << "8. UI/UX Design\n";

    cout << "\nEnter Your Choice: ";
    cin >> choice;

    string selectedSkill;

    switch (choice)
    {
    case 1:
        selectedSkill = "C++";
        break;

    case 2:
        selectedSkill = "HTML CSS JavaScript";
        break;

    case 3:
        selectedSkill = "WordPress";
        break;

    case 4:
        selectedSkill = "Python";
        break;

    case 5:
        selectedSkill = "Database";
        break;

    case 6:
        selectedSkill = "Networking";
        break;

    case 7:
        selectedSkill = "Cyber Security";
        break;

    case 8:
        selectedSkill = "UI/UX Design";
        break;

    default:
        cout << "\nInvalid Skill Choice!\n";
        return;
    }

    if (!currentUsername.empty())
    {
        if (setSkill(currentUsername, selectedSkill))
            cout << "\nSkill Saved Successfully!\n";
        else
            cout << "\nUnable to save skill.\n";
    }
    else
    {
        skill = selectedSkill;

        cout << "\nSkill selected: "
             << selectedSkill << "\n";
    }
}

void Career::careerRecommendation()
{
    if (skill.empty())
    {
        cout << "\nPlease complete Skill Assessment first.\n";
        return;
    }

    career = getCareer(skill);

    cout << "\n========== Career Recommendation ==========\n";
    cout << "Your Skill: " << skill << '\n';
    cout << "Recommended Career: " << career << '\n';
}

void Career::careerMatchScore()
{
    if (career.empty())
    {
        cout << "\nPlease get a Career Recommendation first.\n";
        return;
    }

    matchScore = getMatchScore(career);

    cout << "\n========== Career Match Score ==========\n";
    cout << "Career: " << career << '\n';
    cout << "Career Match Score: "
         << matchScore << "%\n";
}
