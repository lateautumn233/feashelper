#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <sys/prctl.h>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

// Edit and Lock a file
template <typename T>
bool Lockvalue(const char *location, T value)
{
    chmod(location, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH);
    std::ofstream fd(location, std::ios::out | std::ios::trunc);
    if (!fd)
    {
        fd.close();
        return false;
    }
    fd << value;
    fd.close();
    chmod(location, S_IRUSR | S_IRGRP | S_IROTH);
    return true;
}

// Run a shell (always dumpsys) and return result.
int Shell(const char *sh, std::string &result);

bool Testfile(const char *location);

// normal AndroidDevice
class AndroidDevice
{
    std::string Name;
    std::string Frontpkgname;
    std::string Headphonestatus;
    bool Screenstatus;

protected:
    static void Headphonemonitor(std::string &status, int second);
    static void Topappmonitor(std::string &Topapp, int second);
    static void Screenstatusmonitor(bool &status, int second);

public:
    AndroidDevice(const char *name);
    std::string getToppkg();
    bool ifScreenon();
    void startHeadphonemonitor(int second);
    void startTopappmonitor(int second);
    void startScreenstatusmonitor(int second);
    bool getHeadphonestatus();
    std::string getTopapp();
    bool getScreenstatus();
};
