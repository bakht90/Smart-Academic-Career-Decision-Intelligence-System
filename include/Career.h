#ifndef CAREER_H
#define CAREER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Career
{
private:
    string currentUsername;
    string skill;
    string career;
    int matchScore;

public:
    Career();

    void setUsername(const string&);

    void addSkill();
    void careerRecommendation();
    void careerMatchScore();

    bool setSkill(const string&, const string&);
    string getSkill(const string&);
    string getCareer(const string&) const;
    int getMatchScore(const string&) const;
};

#endif
