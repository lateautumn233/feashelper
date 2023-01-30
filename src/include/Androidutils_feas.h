#pragma once

#include "Androidutils.h"

// Feas Android device
class AndroidDeviceFEAS : public AndroidDevice
{
    bool Feas_status;
    bool isFEASSupported;
    std::string type;
public:
    AndroidDeviceFEAS(const char *name);
    static bool getmtkFEASsupport();
    static bool getqcomFEASsupport();
    bool checkFEASType();
    bool FEASon(unsigned int &fps);
    bool FEASoff();
    bool HasFEAS() const;
    std::string getType() const;
};
