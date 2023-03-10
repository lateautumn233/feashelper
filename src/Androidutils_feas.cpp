#include "include/Androidutils_feas.h"
#include "include/fpsgo.h"


AndroidDeviceFEAS::AndroidDeviceFEAS()
{
    isFEASSupported = checkFEASType();
    if (checkFEASType())
        FEASoff();
}

bool AndroidDeviceFEAS::getmtkFEASsupport()
{
    return Testfile("/sys/module/mtk_fpsgo/parameters/perfmgr_enable");
}

bool AndroidDeviceFEAS::getqcomFEASsupport()
{
    return Testfile("/sys/module/perfmgr/parameters/perfmgr_enable");
}

bool AndroidDeviceFEAS::getoldqcomFEASsupport()
{
    return Testfile("/sys/module/perfmgr_policy/parameters/perfmgr_enable");
}

bool AndroidDeviceFEAS::getBocchiFASsupport() {
    return Testfile("/sys/module/bocchi_perfmgr/parameters/perfmgr_enable");
}

bool AndroidDeviceFEAS::checkFEASType()
{
    if (getBocchiFASsupport())
    {
        type = "bocchi";
        return true;
    }
    if (getqcomFEASsupport())
    {
        type = "qcom";
        return true;
    }
    if (getmtkFEASsupport())
    {
        type = "mtk";
        return true;
    }
    if (getoldqcomFEASsupport())
    {
        type = "old_qcom";
        return true;
    }
    return false;
}

static void target_fps_helper_mtk(unsigned int &fps)
{
    if (fps > 100)
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_121", 1);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_91", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_61", 0);
    }
    else if (fps < 100 && fps > 70)
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_121", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_91", 1);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_61", 0);
    }
    else
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_121", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_91", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_61", 1);
    }
}

static void target_fps_helper_qcom(unsigned int &fps)
{
    if (fps > 100)
    {
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_121", 1);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_91", 0);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_61", 0);
    }
    else if (fps < 100 && fps > 70)
    {
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_121", 0);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_91", 1);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_61", 0);
    }
    else
    {
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_121", 0);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_91", 0);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_61", 1);
    }
}

bool AndroidDeviceFEAS::FEASon(unsigned int &fps)
{
    if (type == "bocchi")
    {
        Lockvalue("/sys/module/bocchi_perfmgr/parameters/perfmgr_enable", 1);
        Lockvalue("/sys/module/bocchi_perfmgr/parameters/fixed_target_fps", fps);
    }
    if (type == "mtk")
    {
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/perfmgr_enable", 1))
            return false;
        Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", fps);
        target_fps_helper_mtk(fps);
    }
    if (type == "qcom")
    {
    	fpsgo_ioctl(1, 0, 0, 1, 1);
    	fpsgo_ioctl(2, 0, 0, 1, 1);
        if (!Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 1))
            return false;
        Lockvalue("/sys/module/perfmgr/parameters/fixed_target_fps", fps);
        target_fps_helper_qcom(fps);
    }
    if (type == "old_qcom")
    {
        if (!Lockvalue("/sys/module/perfmgr_policy/parameters/perfmgr_enable", 1))
            return false;
        Lockvalue("/sys/module/perfmgr_policy/parameters/fixed_target_fps", fps);
    }
    return true;
}

bool AndroidDeviceFEAS::FEASoff()
{
    if (type == "bocchi")
    {
        Lockvalue("/sys/module/bocchi_perfmgr/parameters/perfmgr_enable", 0);
        Lockvalue("/sys/module/bocchi_perfmgr/parameters/fixed_target_fps", 0);
    }
    if (type == "mtk")
    {
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/perfmgr_enable", 0))
            return false;
        Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_121", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_91", 0);
        Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_61", 0);
    }
    if (type == "qcom")
    {
    	fpsgo_ioctl(3, 0, 1, 1, 1);
        if (!Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 0))
            return false;
        Lockvalue("/sys/module/perfmgr/parameters/fixed_target_fps", 0);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_61", 0);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_91", 0);
        Lockvalue("/sys/module/perfmgr/parameters/target_fps_121", 0);
    }
    if (type == "old_qcom")
    {
        if (!Lockvalue("/sys/module/perfmgr_policy/parameters/perfmgr_enable", 0))
            return false;
        Lockvalue("/sys/module/perfmgr_policy/parameters/fixed_target_fps", 0);
    }
    return true;
}

bool AndroidDeviceFEAS::HasFEAS() const
{
    return isFEASSupported;
}

std::string AndroidDeviceFEAS::getType() const
{
    return type;
}
