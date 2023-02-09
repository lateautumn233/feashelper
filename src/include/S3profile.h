#pragma once

#include <string>

class listProfile
{
    const char *Profilelocation;
    std::string *p;
    unsigned int line;
    static void Profilemonitor(const unsigned int& second, unsigned int &_line, std::string *&_p, const char *&Profilelocation, bool &performance_governor);

public:
    bool Readsuccess = false;
    unsigned int fps;
    bool performance_governor;
    listProfile(const char *location);
    void startProfilemonitor(const unsigned int& second);
    void List();
    bool Inlist(const std::string &app);
};
