#pragma once

#include "Androidutils.h"

// Feas Android device
class AndroidDeviceFEAS : public AndroidDevice
{
    bool isFEASSupported;
    std::string type;
public:
    AndroidDeviceFEAS();
    AndroidDeviceFEAS(const AndroidDeviceFEAS &others) = delete; // dont't copy this
    static bool getmtkFEASsupport();
    static bool getqcomFEASsupport();
    static bool getoldqcomFEASsupport();
    static bool getBocchiFASsupport();
    bool checkFEASType();
    bool FEASon(unsigned int &fps);
    bool FEASoff();
    bool HasFEAS() const;
    std::string getType() const;
};
