#include <iostream>
#include <unistd.h>
#include <thread>

#include "include/Androidutils_feas.h"
#include "include/S3profile.h"
#include "include/Addutils.h"
#include "include/Uperfhelper.h"

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

static void setGov_normal(AndroidDeviceFEAS &device)
{
    const std::string target = device.getType() == "qcom" ? "walt" : "schedutil";
    const std::string path = "/sys/devices/system/cpu/cpufreq/policy0/scaling_governor";
    while (getGov() != target)
    {
        setGov(target);
        Lockvalue(path, target);
    }
}

void restore(AndroidDeviceFEAS &device) // restore edition from uperf
{
    setGov_normal(device);
    static std::ifstream freq;
    static std::string tmp1,tmp2;
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
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
    Shell("grep \"Use CpufreqWriterPerformance\" /storage/emulated/0/Android/yc/uperf/uperf_log.txt", uperf_gov); // if uperf using performance governor
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
                restore(device);
                restore(device); // 3 time for fully restore uperf's edit
                restored = true;
            }
            
            // uperf
            uperf_stop = true;
            
            // set governor
            if (device.getType() == "old_qcom") // perfmgr_policy can't use performance
                continue;

            if (profile.performance_governor)
            {
                setGov("performance");
            }

            // Additional configuration
            // addutils(device.getToppkg());
        }
        else
        {
            // close feas
            device.FEASoff();
            if (!uperf_stat)
            {
                setGov_normal(device);
                restore(device);
            }
            else
            {
                setGov(uperf_gov);
                Lockvalue("/sys/devices/system/cpu/cpufreq/policy0/scaling_governor", uperf_gov); // back 2 uperf
            }
            // uperf
            uperf_stop = false;
            restored = false;
        }
        // package name update in fastest 1s
        sleep(1);
    }
}
