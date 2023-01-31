#pragma once

#include <fstream>
#include <string>
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
/*template <>
bool Lockvalue(const char *location, const char *value)
{
    chmod(location, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH);
    std::fstream fd(location, std::ios::out | std::ios::trunc);
    if (!fd)
    {
        fd.close();
        return false;
    }
    std::string test;
    do {
        fd << value;
        fd >> test;
    } while(test != value);
    fd.close();
    chmod(location, S_IRUSR | S_IRGRP | S_IROTH);
    return true;
}*/

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
