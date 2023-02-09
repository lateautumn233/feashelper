#pragma once

#include "Androidutils_feas.h"

std::string getGov();
void restore(AndroidDeviceFEAS &device);
void setGov(std::string governor);
void setGov_normal(AndroidDeviceFEAS &device);