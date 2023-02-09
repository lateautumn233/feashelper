#include <thread>
#include <fstream>

#include "include/FreqandGov.h"

std::string getGov()
{
    std::ifstream fd("/sys/devices/system/cpu/cpufreq/policy7/scaling_governor");
    std::string gov;
    fd >> gov;
    return gov;
}

void setGov(std::string governor) // switch cpu to target governor
{
    std::string tmp;
    for (int i = 1; i < 10; i++)
    {
        tmp = "/sys/devices/system/cpu/cpufreq/policy" + std::to_string(i) + "/scaling_governor";
        Lockvalue(tmp, governor);
    }
}

void setGov_normal(AndroidDeviceFEAS &device)
{
    const std::string path = "/sys/devices/system/cpu/cpufreq/policy0/scaling_governor";
    setGov("walt");
    Lockvalue(path, "walt");
    if (getGov() != "walt")
    {
        setGov("schedutil");
        Lockvalue(path, "schedutil");
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