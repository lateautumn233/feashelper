#pragma once

#include <string>

int Countline(const char *location);
bool readProfile(const char *Profilelocation, std::string *&p);

class listProfile
{
    const char *Profilelocation;
    std::string *p;
    int line;

public:
    bool Readsuccess = false;
    int fps;
    listProfile(const char *location);
    static void Profilemonitor(int second, int &_line, std::string *&_p, const char *&Profilelocation);
    void startProfilemonitor(int second);
    void List();
    bool Inlist(std::string app);
};
