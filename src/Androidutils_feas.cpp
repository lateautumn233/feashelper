#include "include/Androidutils.h"
#include "include/Androidutils_feas.h"

bool AndroidDeviceFeas::getmtkFeassupport()
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
bool AndroidDeviceFeas::getqcomFeassupport()
{
    if (!Testfile("/sys/module/perfmgr/parameters/perfmgr_enable"))
        return false;
    if (!Testfile("/sys/module/perfmgr/parameters/target_fps_61"))
        return false;
    return true;
}
bool AndroidDeviceFeas::getFeassupport()
{
    if (getmtkFeassupport())
    {
        type = "mtk";
        return true;
    }
    if (getqcomFeassupport())
    {
        type = "qcom";
        return true;
    }
    return false;
}

AndroidDeviceFeas::AndroidDeviceFeas(const char *name) : AndroidDevice(name)
{
    Feas_status = false;
    Feas_support = getFeassupport();
    if (getFeassupport())
        Feasoff();
}
bool AndroidDeviceFeas::Feason(int &fps)
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
            if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_61", fps))
                tmpbool = false;
        }
    }
    if (type == "qcom")
    {
        if (!Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 1))
            tmpbool = false;
        if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", fps))
        {
            if (!Lockvalue("/sys/module/mtk_fpsgo/parameters/target_fps_61", fps))
                tmpbool = false;
        }
    }
    Feas_status = true;
    return tmpbool;
}
bool AndroidDeviceFeas::Feasoff()
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
bool AndroidDeviceFeas::ifFeas_support() const
{
    return Feas_support;
}
std::string AndroidDeviceFeas::getType() const
{
    return type;
}
