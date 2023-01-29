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
inline bool Lockvalue(const char *location, T value)
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
inline int Shell(const char *sh, std::string &result)
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

inline bool Testfile(const char *location)
{
    std::ifstream fd(location);
    if (!fd)
    {
        return false;
    }
    return true;
}

// normal AndroidDevice
class roidDevice
{

    // friend
    friend int Shell(const char *sh, std::string &result);

protected:
    std::string Name;
    std::string Frontpkgname;
    std::string Headphonestatus;
    bool Screenstatus;

protected:
    static void Headphonemonitor(std::string &status, int second)
    {
        prctl(PR_SET_NAME, "Headphonemonitor");
        while (true)
        {
            Shell("getprop | grep persist.audio.headset.plug.status|cut -d \" \" -f2", status);
            sleep(second);
        }
    }
    static void Topappmonitor(std::string &Topapp, int second)
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
    static void Screenstatusmonitor(bool &status, int second)
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

public:
    roidDevice(const char *name)
    {
        Name = name;
        Screenstatus = false;
        Frontpkgname = "com.unKnown.pkg";
    }
    inline std::string getToppkg()
    {
        return Frontpkgname;
    }
    inline bool ifScreenon()
    {
        return Screenstatus;
    }
    void startHeadphonemonitor(int second)
    {
        std::thread Headphonehelper(Headphonemonitor, std::ref(Headphonestatus), second);
        Headphonehelper.detach();
    }
    void startTopappmonitor(int second)
    {
        std::thread Topapphelper(Topappmonitor, std::ref(Frontpkgname), second);
        Topapphelper.detach();
    }
    void startScreenstatusmonitor(int second)
    {
        std::thread Screenstatushelper(Screenstatusmonitor, std::ref(Screenstatus), second);
    }
    inline bool getHeadphonestatus()
    {
        if (Headphonestatus == "[off]" || Headphonestatus == "")
            return false;
        return true;
    }
    inline std::string getTopapp()
    {
        return Frontpkgname;
    }
    inline bool getScreenstatus()
    {
        return Screenstatus;
    }
};
