#include "include/Androidutils_feas.h"


AndroidDeviceFEAS::AndroidDeviceFEAS(const char *name) : AndroidDevice(name)
{
    Feas_status = false;
    isFEASSupported = checkFEASType();
    if (checkFEASType())
        FEASoff();
}

bool AndroidDeviceFEAS::getmtkFEASsupport()
{
    if (!Testfile("/sys/kernel/fpsgo/common/fpsgo_enable"))
        return false;
    if (!Testfile("/sys/module/mtk_fpsgo/parameters/perfmgr_enable"))
        return false;
    if (!Testfile("/sys/module/mtk_fpsgo/parameters/fixed_target_fps"))
    {
        if (!Testfile("/sys/module/mtk_fpsgo/parameters/target_fps_61"))
            return false;
    }
    return true;
}

bool AndroidDeviceFEAS::getqcomFEASsupport()
{
    if (!Testfile("/sys/module/perfmgr/parameters/perfmgr_enable"))
        return false;
    if (!Testfile("/sys/module/perfmgr/parameters/target_fps_61"))
        return false;
    return true;
}

bool AndroidDeviceFEAS::checkFEASType()
{
    if (getmtkFEASsupport())
    {
        type = "mtk";
        return true;
    }
    if (getqcomFEASsupport())
    {
        type = "qcom";
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
    bool tmpbool = true;
    if (type == "mtk")
    {
        if (!Lockvalue("/sys/kernel/fpsgo/common/fpsgo_enable", 1))
            tmpbool = false;
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/perfmgr_enable", 1))
            tmpbool = false;
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", fps))
        {
            target_fps_helper_mtk(fps);
        }
    }
    if (type == "qcom")
    {
        if (!Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 1))
            tmpbool = false;
        if (!Lockvalue("/sys/module/perfmgr/parameters/fixed_target_fps", fps))
        {
            target_fps_helper_qcom(fps);
        }
    }
    Feas_status = true;
    return tmpbool;
}

bool AndroidDeviceFEAS::FEASoff()
{
    bool tmpbool = true;
    if (type == "mtk")
    {
        if (!Lockvalue("/sys/kernel/fpsgo/common/fpsgo_enable", 0))
            tmpbool = false;
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/perfmgr_enable", 0))
            tmpbool = false;
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", 0))
        {
            if (!Lockvalue("/sys/module/perfmgr/parameters/target_fps_61", 0))
                tmpbool = false;
        }
    }
    if (type == "qcom")
    {
        if (!Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 0))
            tmpbool = false;
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", 0))
        {
            if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_61", 0))
                tmpbool = false;
        }
    }
    Feas_status = false;
    return tmpbool;
}

bool AndroidDeviceFEAS::HasFEAS() const
{
    return isFEASSupported;
}

std::string AndroidDeviceFEAS::getType() const
{
    return type;
}
