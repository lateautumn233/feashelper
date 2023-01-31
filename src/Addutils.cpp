#include <iostream>
#include <fstream>
#include <string>

#include "include/Addutils.h"
#include "include/Androidutils.h"

enum model_list {
    UNKNOWN,
    DITING // k50u, 12tpro
} model;

static short int getModel()
{
    std::string device;
    Shell("getprop ro.product.device", device);
    if(device == "diting")
        return DITING;
    else
        return UNKNOWN;
}

static void diting(std::string &Frontpkgname)
{
    if(Frontpkgname == "com.miHoYo.GenshinImpact" || Frontpkgname == "com.miHoYo.Yuanshen" || Frontpkgname == "com.miHoYo.ys.mi" || Frontpkgname == "com.miHoYo.ys.bilibili")
    {
        Lockvalue("/sys/module/perfmgr/parameters/load_scaling_y", -1);
        Lockvalue("/sys/module/perfmgr/parameters/load_scaling_x", 3);
        Lockvalue("/sys/devices/system/cpu/cpu7/core_ctl/enable", 0);
        Lockvalue("/sys/devices/system/cpu/cpu4/core_ctl/enable", 0);
        Lockvalue("/sys/devices/system/cpu/cpu4/cpufreq/walt/target_loads", 5);
        Lockvalue("/sys/devices/system/cpu/cpu7/cpufreq/walt/target_loads", 5);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a", 300);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_b", -40);
        Lockvalue("/dev/cpuset/top-app/cpus", "4-7");
        Lockvalue("/dev/cpuset/foreground/cpus", "0-7");
        Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 7);
        Lockvalue("/sys/module/perfmgr/parameters/continus_no_jank_count", 14);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_61", 1);
        Lockvalue("/sys/devices/system/cpu/cpu0/cpufreq/walt/rtg_boost_freq", 900000);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a_thres", 950);
        Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 1);
        Lockvalue("/sys/module/perfmgr/parameters/load_scaling_y", 1);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_61", 0);
        Lockvalue("/sys/module/perfmgr/parameters/continus_no_jank_count", 0);
        Lockvalue("/sys/module/perfmgr/parameters/load_scaling_x", 5);
        Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 0);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a", 310);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_b", -50);
        Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 8);
        Lockvalue("/sys/devices/system/cpu/cpu7/core_ctl/enable", 1);
        Lockvalue("/sys/devices/system/cpu/cpu4/core_ctl/enable", 1);
        Lockvalue("/sys/devices/system/cpu/cpu4/cpufreq/walt/target_loads", 0);
        Lockvalue("/sys/devices/system/cpu/cpu7/cpufreq/walt/target_loads", 0);
        Lockvalue("/dev/cpuset/top-app/cpus", "0-7");
        Lockvalue("/dev/cpuset/foreground/cpus", "0-2,4-7");
        Lockvalue("/sys/devices/system/cpu/cpu0/cpufreq/walt/rtg_boost_freq", 1000000);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a_thres", 640);
        Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 0);
    }
}

void addutils(std::string Frontpkgname)
{
    short int device = getModel();
    switch(device)
    {
        case UNKNOWN:
            break;
        case DITING :
            diting(Frontpkgname);
            break;
    }
}