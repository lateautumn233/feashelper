#pragma once

#include "Androidutils.h"

// Feas Android device
class AndroidDeviceFEAS : public AndroidDevice
{
    bool isFEASSupported;
    std::string type;
public:
    AndroidDeviceFEAS();
    static bool getmtkFEASsupport();
    static bool getqcomFEASsupport();
    static bool getoldqcomFEASsupport();
    bool checkFEASType();
    bool FEASon(unsigned int &fps);
    bool FEASoff();
    bool HasFEAS() const;
    std::string getType() const;
};
