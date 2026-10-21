#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Dashboard
{
public:
    void performanceAnalyzer(int, float, int); //attendance, gpa, pending assignment
    void smartAlerts(int, float, int);
    void performanceLevel(int, float);

    int performanceScore(int, float, int) const;
    string performanceStatus(int, float, int) const;
    string performanceLevelText(int, float) const;
    vector<string> alertMessages(int, float, int) const;
};

#endif
