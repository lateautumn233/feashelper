#pragma once
#include "Androidutils.h"

// Feas Android device
class AndroidDeviceFeas : public AndroidDevice
{
    bool Feas_status;
    bool Feas_support;
    std::string type;
public:
    bool getmtkFeassupport();
    bool getqcomFeassupport();
    bool getFeassupport();
    AndroidDeviceFeas(const char *name);
    bool Feason(int &fps);
    bool Feasoff();
    bool ifFeas_support() const;
    std::string getType() const;
};
