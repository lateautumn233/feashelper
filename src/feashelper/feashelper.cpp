#include "../common/Androidutils_feas.h"
#include "../common/S3profile.h"

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
    
    // creat device
    roidDeviceFeas device("mtk-feas");

    // test feas support
    if (!device.ifFeas_support())
    {
        std::cout << "Not support feas, or this sortware is out of date." << std::endl;
        return 1;
    }
    std::cout << "Support." << std::endl;

    //start Topappmonitor
    device.startTopappmonitor(3);
    while(true)
    {
        if(profile.Inlist(device.getToppkg()))
        {
            device.Feason(profile.fps);
            //std::cout << "on" << std::endl;
        }
        else
        {
            //std::cout << "off" << std::endl;
            device.Feasoff();
        }
        sleep(3);
    }
}