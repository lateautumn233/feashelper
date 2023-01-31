#include <iostream>
#include <thread>
#include <sys/prctl.h>
#include <unistd.h>
#include <sys/types.h>

#include "include/Androidutils.h"

// Run a shell (always dumpsys) and return result.
int Shell(const char *sh, std::string &result)
{
    FILE *pp = popen(sh, "r");
    char buf[1024];
    if (pp == NULL)
    {
        perror("Failed");
        return 0;
    }
    // collect result
    fgets(buf, sizeof(buf), pp);
    result = buf;
    result.pop_back();
    pclose(pp);
    return 1;
}

bool Testfile(const char *location)
{
    int ret = access(location, F_OK);
    if (ret == 0)
        return true;
    else
        return false;
}

AndroidDevice::AndroidDevice(const char *name)
{
    Name = name;
    Frontpkgname = "com.unKnown.pkg";
}


void AndroidDevice::Topappmonitor(std::string &Topapp, unsigned int second)
{
    prctl(PR_SET_NAME, "Topappmonitor");
    while (true)
    {
        Shell("dumpsys activity activities|grep topResumedActivity=|tail -n 1|cut -d \"{\" -f2|cut -d \"/\" -f1|cut -d \" \" -f3", Topapp);
        // reduce screen off cost
        if (Topapp.empty())
            sleep(6);
        sleep(second);
    }
}

std::string AndroidDevice::getToppkg()
{
    return Frontpkgname;
}

void AndroidDevice::startTopappmonitor(unsigned int second)
{
    std::thread Topapphelper(Topappmonitor, std::ref(Frontpkgname), second);
    Topapphelper.detach();
}

std::string AndroidDevice::getTopapp()
{
    return Frontpkgname;
}
