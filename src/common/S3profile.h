#pragma once
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <sys/prctl.h>

int Countline(const char *location)
{
    std::ifstream cfgFile(location);
    if (!cfgFile)
    {
        return 0;
    }
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
    {
        return false;
    }
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
class listProfile
{

    friend int Countline(const char *location);
    friend bool readProfile(const char *Profilelocation, std::string *&_p);

private:
    const char *Profilelocation;
    std::string *p;
    int line;

public:
    bool Readsucces = false;
    int fps;

public:
    listProfile(const char *location)
    {
        Profilelocation = location;
        line = Countline(Profilelocation);
        p = new std::string[line];
        if (readProfile(Profilelocation, p))
            Readsucces = true;
    }
    static void Profilemonitor(int second, int &_line, std::string *&_p, const char *&Profilelocation)
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
    void startProfilemonitor(int second)
    {
        std::thread Profilehelper(Profilemonitor, second, std::ref(line), std::ref(p), std::ref(Profilelocation));
        Profilehelper.detach();
    }
    inline void List()
    {
        for (int i = 0; i < line; i++)
        {
            std::cout << *(p + i) << std::endl;
        }
    }
    bool Inlist(std::string app)
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
};
