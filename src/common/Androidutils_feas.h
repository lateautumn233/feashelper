#pragma once
#include "Androidutils.h"
// Feas Android device
class roidDeviceFeas : public roidDevice
{

private:
    bool Feas_status;
    bool Feas_support;
    std::string type;
private:
    bool getmtkFeassupport()
    {
        if (!Testfile("/sys/kernel/fpsgo/common/fpsgo_enable"))
            return false;
        if (!Testfile("/sys/module/mtk_fpsgo/parameters/perfmgr_enable"))
            return false;
        if(!Testfile("/sys/module/mtk_fpsgo/parameters/fixed_target_fps"))
            return false;
        return true;
    }
    bool getqcomFeassupport()
    {
        if (!Testfile("/sys/module/perfmgr/parameters/perfmgr_enable"))
            return false;
        if(!Testfile("/sys/module/perfmgr/parameters/target_fps_61"))
            return false;
        return true;
    }
    bool getFeassupport()
    {
        if(getmtkFeassupport())
        {
            type = "mtk";
            return true;
        }
        if(getqcomFeassupport())
        {
            type = "qcom";
            return true;
        }
        return false;
    }
public:
    roidDeviceFeas(const char *name) : roidDevice(name)
    {
        Feas_status = false;
        Feas_support = getFeassupport();
        if(getFeassupport())
            Feasoff();
    }
    inline bool Feason(int &fps)
    {
        if(type == "mtk")
        {
            if(!Lockvalue("/sys/kernel/fpsgo/common/fpsgo_enable", 1))
                return false;
            if(!Lockvalue("/sys/module/mtk_fpsgo/parameters/perfmgr_enable", 1))
                return false;
            if(!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", fps))
                return false;
        }
        if(type == "qcom")
        {
            if(!Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 1))
                return false;
            if(!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", fps))
            {
                if(!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps_61", fps))
                    return false;
            }
        }
        Feas_status = true;
        return true;
    }
    inline bool Feasoff()
    {
        if(type == "mtk")
        {
            if(!Lockvalue("/sys/kernel/fpsgo/common/fpsgo_enable", 0))
                return false;
            if(!Lockvalue("/sys/module/mtk_fpsgo/parameters/perfmgr_enable", 0))
                return false;
            if(!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps", 0))
                return false;
        }
        if(type == "qcom")
        {
            if(!Lockvalue("/sys/module/perfmgr/parameters/perfmgr_enable", 0))
                return false;
            if(!Lockvalue("/sys/module/mtk_fpsgo/parameters/fixed_target_fps_61", 0))
                return false;
        }
        Feas_status = false;
        return true;
    }
    inline bool ifFeas_support()
    {
        return Feas_support;
    }
};
