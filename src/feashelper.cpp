#include <iostream>
#include <unistd.h>

#include "include/Androidutils_feas.h"
#include "include/S3profile.h"
#include "include/Addutils.h"
#include "include/uperfhelper.h"

static std::string getGov()
{
    std::ifstream fd("/sys/devices/system/cpu/cpufreq/policy7/scaling_governor");
    std::string gov;
    fd >> gov;
    return gov;
}

static void setGov(std::string governor) // switch cpu to target governor
{
    std::string tmp;
    for (int i = 1; i < 10; i++)
    {
        tmp = "/sys/devices/system/cpu/cpufreq/policy" + std::to_string(i) + "/scaling_governor";
        Lockvalue(tmp, governor);
    }
}

void setgov_normal(AndroidDeviceFEAS &device)
{
// swich performance to schedutil/walt
    while (getGov() != "walt" && getGov() != "schedutil")
    {
        if (device.getType() == "qcom")
        {
            setGov("walt");
            Lockvalue("/sys/devices/system/cpu/cpufreq/policy0/scaling_governor", "walt");
            if (getGov() != "walt") // fall back
            {
                setGov("schedutil");
                Lockvalue("/sys/devices/system/cpu/cpufreq/policy0/scaling_governor", "schedutil");
            }
        }
        if (device.getType() == "mtk")
        {
            setGov("schedutil");
            Lockvalue("/sys/devices/system/cpu/cpufreq/policy0/scaling_governor", "schedutil");
        }
    }
}

void restore(AndroidDeviceFEAS &device)
{
    setgov_normal(device);
    std::ifstream freq;
    std::string tmp1,tmp2;
    for (int i = 0; i < 10; i++)
    {
        tmp1 = "/sys/devices/system/cpu/cpufreq/policy" + std::to_string(i) + "/cpuinfo_max_freq";
        freq.open(tmp1.c_str());
        if(freq)
        {
            freq >> tmp2;
            tmp1 = "/sys/devices/system/cpu/cpufreq/policy" + std::to_string(i) + "/scaling_max_freq";
            Lockvalue(tmp1, tmp2);
        }
        freq.close();
        tmp1 = "/sys/devices/system/cpu/cpufreq/policy" + std::to_string(i) + "/cpuinfo_min_freq";
        freq.open(tmp1.c_str());
        if(freq)
        {
            freq >> tmp2;
            tmp1 = "/sys/devices/system/cpu/cpufreq/policy" + std::to_string(i) + "/scaling_min_freq";
            Lockvalue(tmp1, tmp2);
        }
        freq.close();
    }
}

int main(int argc, char *argv[])
{
    std::cout.sync_with_stdio(false);
    std::cout << std::unitbuf;
    std::cout << argv[0] << ": Start running\n";

    // read profile
    const char *Profilelocat = argv[1];
    listProfile profile(Profilelocat);
    if (!profile.Readsuccess)
    {
        // read error
        std::cout << "Profile error\n";
        return 1;
    }
    std::cout << "Read profile done.\n";
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
    std::cout << "Device is supported.\n";
    std::cout << "Device category: " << device.getType() << '\n';

    // start Topappmonitor
    device.startTopappmonitor(3);
    
    // start uperfhelper
    bool uperf_stop = true, uperf_stat = false;
    startUperfhelper(uperf_stat, uperf_stop);
    
    std::string uperf_gov;
    Shell("grep \"Use CpufreqWriterPerformance\" /storage/emulated/0/Android/yc/uperf/uperf_log.txt", uperf_gov);
    if (uperf_gov.empty())
        uperf_gov = "powersave";
    else
        uperf_gov = "performance";
    bool restored = false;
    
    while (true)
    {
        if (profile.Inlist(device.getToppkg())) // is a game in config
        {
            // open feas
            device.FEASon(profile.fps);
            if (!restored)
            {
                restore(device);
                restored = true;
            }
            
            // uperf
            uperf_stop = true;
            
            // set governor
            if (profile.performance_governor)
            {
                setGov("performance");
            }

            // Additional configuration
            addutils(device.getToppkg());
        }
        else
        {
            // close feas
            device.FEASoff();
            if (!uperf_stat)
            {
                setgov_normal(device);
                restore(device);
            }
            else
            {
                setGov(uperf_gov);
                Lockvalue("/sys/devices/system/cpu/cpufreq/policy0/scaling_governor", uperf_gov);
            }
            // uperf
            uperf_stop = false;
            restored = false;
        }
        // dumpsys update in 3s
        sleep(3);
    }
}
