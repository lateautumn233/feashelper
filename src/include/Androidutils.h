#pragma once

#include <fstream>
#include <string>
#include <sys/stat.h>

// Edit and Lock a file
template <typename T>
bool Lockvalue(std::string location, T value)
{
    chmod(location.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH);
    std::ofstream fd(location.c_str(), std::ios::out | std::ios::trunc);
    if (!fd)
    {
        fd.close();
        return false;
    }
    fd << value;
    fd.close();
    chmod(location.c_str(), S_IRUSR | S_IRGRP | S_IROTH);
    return true;
}

int Shell(const char *sh, std::string &result);
bool Testfile(const char *location);

// normal AndroidDevice
class AndroidDevice
{
    std::string Name;
    std::string Frontpkgname;

protected:
    static void Topappmonitor(std::string &Topapp, unsigned int second);

public:
    AndroidDevice(const char *name);
    std::string getToppkg();
    bool ifScreenon();
    void startTopappmonitor(unsigned int second);
    std::string getTopapp();
};
