#include <iostream>
#include <thread>
#include <string>
#include <sys/prctl.h>
#include <unistd.h>
#include <sys/types.h>

#include "include/Androidutils.h"

// Run a shell (always dumpsys) and return result.
bool Shell(const char *sh, std::string &result)
{
    FILE *pp = popen(sh, "r");
    if (pp == NULL)
    {
        perror("Failed");
        return false;
    }
    char buff[1024];
    // collect result
    fgets(buff, sizeof(buff), pp);
    result = buff;
    result.pop_back();
    pclose(pp);
    return true;
}

bool Testfile(const char* location)
{
    int ret = access(location, F_OK);
    return (ret == 0);
}

AndroidDevice::AndroidDevice()
{
    Frontpkgname = "com.unKnown.pkg";
}


void AndroidDevice::Topappmonitor(std::string &Topapp, unsigned int second)
{
    prctl(PR_SET_NAME, "Topappmonitor");
    while (true)
    {
        if (Testfile("/sys/kernel/gbe/gbe2_fg_pid"))
        {
            std::string pid;
            std::ifstream Pid("/sys/kernel/gbe/gbe2_fg_pid"), app;
            Pid >> pid;
            Pid.close();
            
            app.open(std::string("/proc/" + pid + "/cmdline").c_str());
            app >> Topapp;
            app.close();
            
            while (Topapp[Topapp.length() - 1] == '\0') // trim
               Topapp.pop_back();
            second = 1;
        }
        else
        {
            Shell("dumpsys activity activities|grep topResumedActivity=|tail -n 1|cut -d \"{\" -f2|cut -d \"/\" -f1|cut -d \" \" -f3", Topapp);
        }
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

void AndroidDevice::startTopappmonitor(const unsigned int& second)
{
    std::thread Topapphelper(Topappmonitor, std::ref(Frontpkgname), second);
    Topapphelper.detach();
}

std::string AndroidDevice::getTopapp()
{
    return Frontpkgname;
}
