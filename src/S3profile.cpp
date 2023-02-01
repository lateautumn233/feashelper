#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <sys/prctl.h>

#include "include/S3profile.h"

static unsigned int Countline(const char *location)
{
    std::ifstream cfgFile(location);
    if (!cfgFile)
        return 0;
    char tmp[1024];
    unsigned int i = 0;
    while (!cfgFile.eof())
    {
        cfgFile.getline(tmp, sizeof(tmp));
        if (tmp[0] != '#')
            i++;
    }
    return i;
}

static bool readProfile(const char *Profilelocation, std::string *&p, bool &performance_governor)
{
    std::ifstream cfgFile(Profilelocation);
    if (!Profilelocation)
        return false;
    char tmp[1024];
    unsigned int i = 0;
    while (!cfgFile.eof())
    {
        cfgFile.getline(tmp, sizeof(tmp));
        if(tmp == std::string("Performance governor = true"))
        {
            performance_governor = true;
        }
        else
        {
            performance_governor = false;
        }
        if (tmp[0] != '#' && tmp[0] != '$')
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
    if (readProfile(Profilelocation, p, performance_governor))
        Readsuccess = true;
}

void listProfile::Profilemonitor(unsigned int second, unsigned int &_line, std::string *&_p, const char *&Profilelocation, bool &performance_governor)
{
    prctl(PR_SET_NAME, "Profilemonitor");
    while (true)
    {
        _line = Countline(Profilelocation);
        _p = new std::string[_line];
        readProfile(Profilelocation, _p, performance_governor);
        sleep(second);
    }
}

void listProfile::startProfilemonitor(unsigned int second)
{
    std::thread Profilehelper(Profilemonitor, second, std::ref(line), std::ref(p), std::ref(Profilelocation), std::ref(performance_governor));
    Profilehelper.detach();
}

void listProfile::List()
{
    for (unsigned int i = 0; i < line; i++)
        std::cout << *(p + i) << '\n';
}

bool listProfile::Inlist(std::string app)
{
    for (unsigned int i = 0; i < line; i++)
    {
        std::size_t pos = (p + i)->find(' ');
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
