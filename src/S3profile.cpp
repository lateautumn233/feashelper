#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <sys/prctl.h>

#include "include/S3profile.h"

int Countline(const char *location)
{
    std::ifstream cfgFile(location);
    if (!cfgFile)
        return 0;
    char tmp[1024];
    int i = 0;
    while (!cfgFile.eof())
    {
        cfgFile.getline(tmp, sizeof(tmp));
        if (tmp[0] != '#')
            i++;
    }
    return i;
}

bool readProfile(const char *Profilelocation, std::string *&p)
{
    std::ifstream cfgFile(Profilelocation);
    if (!Profilelocation)
        return false;
    char tmp[1024];
    int i = 0;
    while (!cfgFile.eof())
    {
        cfgFile.getline(tmp, sizeof(tmp));
        if (tmp[0] != '#')
        {
            *(p + i) = tmp;
            i++;
        }
    }
    return true;
}

listProfile::listProfile(const char *location)
{
    Profilelocation = location;
    line = Countline(Profilelocation);
    p = new std::string[line];
    if (readProfile(Profilelocation, p))
        Readsuccess = true;
}

void listProfile::Profilemonitor(int second, int &_line, std::string *&_p, const char *&Profilelocation)
{
    prctl(PR_SET_NAME, "Profilemonitor");
    while (true)
    {
        _line = Countline(Profilelocation);
        _p = new std::string[_line];
        readProfile(Profilelocation, _p);
        sleep(second);
    }
}

void listProfile::startProfilemonitor(int second)
{
    std::thread Profilehelper(Profilemonitor, second, std::ref(line), std::ref(p), std::ref(Profilelocation));
    Profilehelper.detach();
}

void listProfile::List()
{
    for (int i = 0; i < line; i++)
        std::cout << *(p + i) << '\n';
}

bool listProfile::Inlist(std::string app)
{
    for (int i = 0; i < line; i++)
    {
        // asdf 120
        int pos = (p + i)->find(' ');
        std::string pkgname = (p + i)->substr(0, pos);
        if (app == pkgname)
        {
            std::string s_fps = (p + i)->substr((pos + 1), ((p + i)->length() - pos));
            fps = atoi(s_fps.c_str());
            return true;
        }
        fps = 120;
    }
    return false;
}