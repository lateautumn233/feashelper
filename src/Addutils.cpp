#include <iostream>
#include <fstream>
#include <string>

#include "include/Addutils.h"
#include "include/Androidutils.h"

enum model_list {
    UNKNOWN,
    DITING, // k50u, 12tpro
    RUBENS = 2, XAGA = 2, MATISSE = 2, DAUMIER = 2// k50, note11tpro, k50pro, 12pro_mtkver
} model;

static short int getModel()
{
    std::string device;
    Shell("getprop ro.product.device", device);
    if(device == "diting")
        return DITING;
    else if(device == "rubens" || device == "xaga" || device == "matisse" || device == "daumier")
        return RUBENS;
    else
        return UNKNOWN;
}

static void diting(std::string &Frontpkgname)
{
    if(Frontpkgname == "com.miHoYo.GenshinImpact" || Frontpkgname == "com.miHoYo.Yuanshen" || Frontpkgname == "com.miHoYo.ys.mi" || Frontpkgname == "com.miHoYo.ys.bilibili")
    {
        Lockvalue("/sys/devices/system/cpu/cpu7/core_ctl/enable", 0);
        Lockvalue("/sys/devices/system/cpu/cpu4/core_ctl/enable", 0);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a", 280);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_b", -40);
        Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 5);
        Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 0);
    }
}

static void rubens(std::string &Frontpkgname)
{
    if(Frontpkgname == "com.miHoYo.GenshinImpact" || Frontpkgname == "com.miHoYo.Yuanshen" || Frontpkgname == "com.miHoYo.ys.mi" || Frontpkgname == "com.miHoYo.ys.bilibili")
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 5);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 280);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_b", -40);
        Lockvalue("/sys/kernel/fpsgo/fbt/switch_idleprefer", 0);
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
        case RUBENS :
            rubens(Frontpkgname);
            break;
    }
}