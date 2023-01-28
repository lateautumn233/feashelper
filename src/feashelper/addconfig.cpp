#include "../common/Androidutils_feas.h"
#include "../common/Androidutils.h"
#include <string>

static inline bool isNewFeas()
{
    if(Testfile("/sys/module/mtk_fpsgo/parameters/target_fps_61"))
    {
        return true;
    }
    else if(Testfile("/sys/module/perfmgr/parameters/target_fps_61"))
    {
        return true;
    }
    return false;
}
const bool NFEAS = isNewFeas();
/*Default*/
static inline void Default(roidDeviceFeas &device)
{
    if(device.getType() == "qcom")
    {
        Lockvalue("/sys/module/perfmgr/parameters/load_scaling_y", 1);
        Lockvalue("/sys/module/perfmgr/parameters/load_scaling_a", 450);
        Lockvalue("/sys/module/perfmgr/parameters/load_scaling_b", int(-70));
        Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 8);
        Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 0);
    }
    else if (device.getType() == "mtk")
    {
        if(isNewFeas())
        {
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 500);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 1);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/load_scaling_x", 5);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 8);
        }
        /*else
        {
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 500);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 1);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/load_scaling_x", 5);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 8);
        }*/
    }
}

/*Genshin*/
static inline void OPdo(roidDeviceFeas &device)
{
    if(device.getType() == "qcom")//qcom
    {
        if(NFEAS)//new feas
        {
            Lockvalue("/sys/module/perfmgr/parameters/load_scaling_y", 1);
            Lockvalue("/sys/module/perfmgr/parameters/load_scaling_a", 280);
            Lockvalue("/sys/module/perfmgr/parameters/load_scaling_b", int(-40));
            Lockvalue("/sys/module/perfmgr/parameters/normal_frame_keep_count", 10);
            Lockvalue("/sys/module/perfmgr/parameters/predict_freq_level", 0);
        }
    }
    if(device.getType() == "mtk")//mtk
    {
        if(NFEAS)//new feas
        {
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 400);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_b", int(-50));
            Lockvalue("/sys/module/mtk_fpsgo/parameters/load_scaling_x", 6);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/load_scaling_y", int(-1));
            Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 10);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/continus_no_jank_count", 15);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a_thres", 800);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 1);
        }
        /*else
        {
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a", 400);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_b", int(-50));
            Lockvalue("/sys/module/mtk_fpsgo/parameters/normal_frame_keep_count", 10);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/scaling_a_thres", 800);
            Lockvalue("/sys/module/mtk_fpsgo/parameters/predict_freq_level", 1);
        }*/
    }
}
static inline bool isOP(roidDeviceFeas &device)
{
    if(device.getToppkg() == std::string("com.miHoYo.GenshinImpact") || device.getToppkg() == std::string("com.miHoYo.Yuanshen") || device.getToppkg() == std::string("com.miHoYo.ys.bilibili") || device.getToppkg() == std::string("com.miHoYo.ys.mi")) //if game is OP
    {
        //std::cout << "Genshin" << std::endl;
        return true;
    }
    return false;
}
/*Genshin*/

void addutils(roidDeviceFeas &device)
{
    //Genshin
    if(isOP(device))
        OPdo(device);
    else
        Default(device);
    //
    
}