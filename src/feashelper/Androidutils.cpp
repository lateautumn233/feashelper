#include <iostream>
#include <string>
#include <thread>
#include <sys/prctl.h>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "../common/Androidutils.h"

// Run a shell (always dumpsys) and return result.
int Shell(const char *sh, std::string &result)
{
    FILE *pp = popen(sh, "r");
    char tmp[1024];
    if (pp == NULL)
    {
        perror("Failed");
        return 0;
    }
    // collect result
    fgets(tmp, sizeof(tmp), pp);
    result = tmp;
    result.erase(result.length() - 1, result.length() - 1);
    pclose(pp);
    return 1;
}

bool Testfile(const char *location)
{
    std::ifstream fd(location);
    if (!fd)
    {
        return false;
    }
    return true;
}

AndroidDevice::AndroidDevice(const char *name)
{
    Name = name;
    Screenstatus = false;
    Frontpkgname = "com.unKnown.pkg";
}

void AndroidDevice::Headphonemonitor(std::string &status, int second)
{
    prctl(PR_SET_NAME, "Headphonemonitor");
    while (true)
    {
        Shell("getprop | grep persist.audio.headset.plug.status|cut -d \" \" -f2", status);
        sleep(second);
    }
}

void AndroidDevice::Topappmonitor(std::string &Topapp, int second)
{
    prctl(PR_SET_NAME, "Topappmonitor");
    while (true)
    {
        Shell("dumpsys activity activities|grep topResumedActivity=|tail -n 1|cut -d \"{\" -f2|cut -d \"/\" -f1|cut -d \" \" -f3", Topapp);
        // reduce screen off cost
        if (Topapp == std::string(""))
        {
            sleep(6);
        }
        sleep(second);
    }
}

void AndroidDevice::Screenstatusmonitor(bool &status, int second)
{
    std::string thestatus;
    prctl(PR_SET_NAME, "Screenstatusmonitor");
    while (true)
    {
        Shell("dumpsys window policy|grep screenState=|cut -d \"=\"", thestatus);
        if (!thestatus.compare("SCREEN_STATE_ON"))
            status = true;
        else
            status = false;
        sleep(second);
    }
}

std::string AndroidDevice::getToppkg()
{
    return Frontpkgname;
}

bool AndroidDevice::ifScreenon()
{
    return Screenstatus;
}

void AndroidDevice::startHeadphonemonitor(int second)
{
    std::thread Headphonehelper(Headphonemonitor, std::ref(Headphonestatus), second);
    Headphonehelper.detach();
}

void AndroidDevice::startTopappmonitor(int second)
{
    std::thread Topapphelper(Topappmonitor, std::ref(Frontpkgname), second);
    Topapphelper.detach();
}

void AndroidDevice::startScreenstatusmonitor(int second)
{
    std::thread Screenstatushelper(Screenstatusmonitor, std::ref(Screenstatus), second);
}

bool AndroidDevice::getHeadphonestatus()
{
    if (Headphonestatus == "[off]" || Headphonestatus == "")
        return false;
    return true;
}

std::string AndroidDevice::getTopapp()
{
    return Frontpkgname;
}

bool AndroidDevice::getScreenstatus()
{
    return Screenstatus;
}