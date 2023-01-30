#include <iostream>
#include <unistd.h>

#include "include/Androidutils_feas.h"
#include "include/S3profile.h"
//#include "addconfig.h"

static std::string getGov()
{
    std::ifstream fd("/sys/devices/system/cpu/cpufreq/policy4/scaling_governor");
    std::string gov;
    fd >> gov;
    fd.close();
    return gov;
}
static void setGov(const char *governor) // switch cpu 4-7 to performance
{
    while (getGov() != governor)
    {
        Lockvalue("/sys/devices/system/cpu/cpufreq/policy4/scaling_governor", governor);
        Lockvalue("/sys/devices/system/cpu/cpufreq/policy7/scaling_governor", governor);
    }
}

int main(int argc, char *argv[])
{
    std::cout << argv[0] << ": Start running" << std::endl;

    // read profile
    const char *Profilelocat = argv[1];
    listProfile profile(Profilelocat);
    if (!profile.Readsuccess)
    {
        // read error
        std::cout << "Profile error" << std::endl;
        return 1;
    }
    std::cout << "Read profile done." << std::endl;
    profile.List();
    // read done

    // update-profile daemon
    profile.startProfilemonitor(5);

    // creat device
    AndroidDeviceFEAS device("feas");

    // test feas support
    if (!device.HasFEAS())
    {
        std::cout << "Unsupported device, or you are using an outdated version of FEASHelper.\n";
        // not supported
        return 1;
    }
    std::cout << "Support.\n";
    std::cout << "Device: " << device.getType() << '\n';

    // start Topappmonitor
    device.startTopappmonitor(3);
    while (true)
    {
        if (profile.Inlist(device.getToppkg())) // is a game in config
        {
            // open feas
            device.FEASon(profile.fps);

            // set governor
            setGov("performance");

            /*From mi joyose config
             *now only genshin*/
            //addutils(device);
        }
        else
        {
            // close feas
            device.FEASoff();

            // swich performance to schedutil/walt
            while (getGov() != "walt" && getGov() != "schedutil")
            {
                if (device.getType() == "qcom")
                {
                    setGov("walt");
                    if (getGov() != "walt") // fall back
                        setGov("schedutil");
                }
                if (device.getType() == "mtk")
                    setGov("schedutil");
            }
        }
        // dumpsys update in 3s
        sleep(3);
    }
}
