#include <algorithm>
#include <array>
#include <fstream>
#include <string>

#include "include/Addutils.h"
#include "include/Androidutils.h"

constexpr std::array OP{"com.miHoYo.GenshinImpact", "com.miHoYo.Yuanshen", "com.miHoYo.ys.mi", "com.miHoYo.ys.bilibili"};
constexpr std::array NP{"com.tencent.tmgp.sgamece", "com.tencent.tmgp.sgame"};
constexpr std::array BP{"com.miHoYo.bh3.mi", "com.tencent.tmgp.bh3", "com.miHoYo.enterprise.NGHSoDBeta", "com.miHoYo.enterprise.NGHSoD"};

static model_list getModel()
{
    std::string device;
    constexpr std::array mtk_devices{"rubens", "xaga", "matisse", "daumier"};
    Shell("getprop ro.product.device", device);
    if (device == "diting")
        return QCOM;
    else if (std::any_of(mtk_devices.cbegin(), mtk_devices.cend(), [&](const auto &it) { return device == it; }))
        return MTK;
    else
        return UNKNOWN;
}

static void qcom(std::string &Frontpkgname)
{
    /* Genshin */
    /* if (std::any_of(OP.cbegin(), OP.cend(), [&](const auto &it) { return Frontpkgname == it; }))
    {
        Lockvalue("/sys/devices/system/cpu/cpu7/core_ctl/enable", 0);
        Lockvalue("/sys/devices/system/cpu/cpu4/core_ctl/enable", 0);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a", 280);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_b", -40);
        Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 5);
        Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 0);
    } */
    /* Glory of Kings */
    /* else if (std::any_of(NP.cbegin(), NP.cend(), [&](const auto &it) { return Frontpkgname == it; }))
    {
        Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 1);
        Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 12);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a", 400);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a_thres", 580);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_b", -75);
    } */
    /* Lolm */
    /*else if (Frontpkgname == "com.tencent.lolm")
    {
        Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 1);
        Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 6);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a", 365);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_a_thres", 500);
        Lockvalue("/sys/module/perfmgr/parameters/scaling_b", -50);
    }*/
}

static void mtk(std::string &Frontpkgname)
{
    /* Genshin */
    /*if (std::any_of(OP.cbegin(), OP.cend(), [&](const auto &it) { return Frontpkgname == it; }))
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 5);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 280);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_b", -40);
        Lockvalue("/sys/kernel/fpsgo/fbt/switch_idleprefer", 0);
    }*/
    /* Glory of Kings */
    /*else if (std::any_of(NP.cbegin(), NP.cend(), [&](const auto &it) { return Frontpkgname == it; }))
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 1);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 12);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 400);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a_thres", 580);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_b", -75);
    }*/
    /* Lolm */
    /*else if (Frontpkgname == "com.tencent.lolm")
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 1);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 6);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 365);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a_thres", 500);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_b", -50);
    }*/
    /* Bh3 */
    /*else if (std::any_of(BP.cbegin(), BP.cend(), [&](const auto &it) { return Frontpkgname == it; }))
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 1);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 8);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 400);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_b", -60);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/load_scaling_x", 5);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/load_scaling_x", 1);
    }*/
}

void addutils(std::string Frontpkgname)
{
    model_list device = getModel();
    switch (device)
    {
    case UNKNOWN:
        break;
    case QCOM:
        qcom(Frontpkgname);
        break;
    case MTK:
        mtk(Frontpkgname);
        break;
    }
}