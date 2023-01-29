#include "../common/Androidutils_feas.h"
#include "../common/S3profile.h"
#include "addconfig.h"

inline std::string getGov(){
    std::ifstream fd("/sys/devices/system/cpu/cpufreq/policy4/scaling_governor");
    std::string gov;
    fd >> gov;
    fd.close();
    return gov;
}
inline void setGov(const char *governor)//swich cpu 0-7 to performance
{
    while(getGov() != std::string(governor))
    {
        Lockvalue("/sys/devices/system/cpu/cpufreq/policy4/scaling_governor", governor);
        Lockvalue("/sys/devices/system/cpu/cpufreq/policy7/scaling_governor", governor);
    }
}
//stop feas boost_affinity if had installed asoulopt
inline void Ifasopt()
{
    if(Testfile("/data/adb/modules/asoul_affinity_opt/AsoulOpt"))
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/boost_affinity", 0);
        Lockvalue("/sys/module/perfmgr/parameters/boost_affinity", 0);
    }
    else
    {
        Lockvalue("/sys/module/mtk_fpsgo/parameters/boost_affinity", 1);
        Lockvalue("/sys/module/perfmgr/parameters/boost_affinity", 1);
    }
}

int main(int argc, char* argv[])
{
    std::cout << argv[0] << ": Start running" << std::endl;

    //read profile
    const char *Profilelocat = argv[1];
    listProfile profile(Profilelocat);
    if(!profile.Readsucces)
    {
        //read error
        std::cout << "Profile error" << std::endl;
        return 1;
    }
    std::cout << "Read profile done." << std::endl;
    profile.List();
    //read done
    
    //update-profile daemon
    profile.startProfilemonitor(5);
    
    //creat device
    roidDeviceFeas device("feas");

    //test feas support
    if (!device.ifFeas_support())
    {
        std::cout << "Device not support, or this sortware is out of date." << std::endl;
        //not support
        return 1;
    }
    std::cout << "Support." << std::endl;
    std::cout << "Device: " << device.getType() << std::endl;

    //start Topappmonitor
    device.startTopappmonitor(3);
    while(true)
    {
        if(profile.Inlist(device.getToppkg()))//is a game in config
        {
            Ifasopt();
            
            //open feas
            device.Feason(profile.fps);

            //set governor
            setGov("performance");
            
            /*From mi joyose config
             *now only genshin*/
            addutils(device);
        }
        else
        {
            //close feas
            device.Feasoff();
            
            //swich performance to schedutil/walt
            while(getGov() != "walt" && getGov() != "schedutil")
            {
                if(device.getType() == "qcom")
                {
                    setGov("walt");
                    if(getGov() != "walt") //fall back
                    {
                        setGov("schedutil");
                    }
                }
                if(device.getType() == "mtk")
                {
                    setGov("schedutil");
                }
            }
        }
        //dumpsys update in 3s
        sleep(3);
    }
}