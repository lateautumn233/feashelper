#pragma once

#include <string>

class listProfile
{
    const char *Profilelocation;
    std::string *p;
    unsigned int line;
    static void Profilemonitor(unsigned int second, unsigned int &_line, std::string *&_p, const char *&Profilelocation);

public:
    bool Readsuccess = false;
    unsigned int fps;
    listProfile(const char *location);
    void startProfilemonitor(unsigned int second);
    void List();
    bool Inlist(std::string app);
};
