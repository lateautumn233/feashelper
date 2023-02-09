#include <iostream>
#include <unistd.h>
#include <thread>

#include "include/Androidutils_feas.h"
#include "include/S3profile.h"
//#include "include/Addutils.h"
#include "include/FreqandGov.h"


int main(int argc, char *argv[])
{
    std::cout.sync_with_stdio(false);
    std::cout << std::unitbuf;
    std::cout << argv[0] << ": Start running\n";

    // read profile
    const char *Profilelocat = argv[1];
    listProfile profile(Profilelocat);
    if (!profile.Readsuccess)
    {
        // read error
        std::cout << "Profile error\n";
        return 1;
    }
    std::cout << "Read profile done.\n";
    profile.List();
    // read done

    // update-profile daemon
    profile.startProfilemonitor(5);

    // creat device
    AndroidDeviceFEAS device;

    // test feas support
    if (!device.HasFEAS())
    {
        std::cout << "Unsupported device, or you are using an outdated version of FEASHelper.\n";
        // not supported
        return 1;
    }
    std::cout << "Device is supported.\n";
    std::cout << "Device category: " << device.getType() << '\n';

    // start Topappmonitor
    device.startTopappmonitor(3);
    
    while (true)
    {
        if (profile.Inlist(device.getToppkg())) // is a game in config
        {
            // open feas
            device.FEASon(profile.fps);
            
            // Additional configuration
            // addutils(device.getToppkg());
            
            // set governor
            if (profile.performance_governor && device.getType() != "old_qcom")
            {
                setGov("performance");
            }
        }
        else
        {
            // close feas
            device.FEASoff();
            setGov_normal(device);
            restore(device);
        }
        // package name update in fastest 1s
        sleep(1);
    }
}
