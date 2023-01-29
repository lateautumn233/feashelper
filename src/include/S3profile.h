#pragma once

#include <string>

class listProfile
{
    const char *Profilelocation;
    std::string *p;
    int line;
    static void Profilemonitor(int second, int &_line, std::string *&_p, const char *&Profilelocation);

public:
    bool Readsuccess = false;
    int fps;
    listProfile(const char *location);
    void startProfilemonitor(int second);
    void List();
    bool Inlist(std::string app);
};
