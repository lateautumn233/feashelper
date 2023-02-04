#pragma once

void startUperfhelper(bool &stat, bool &stop);

class Uperf
{
    unsigned int uperf_pid;
    bool stopped;  
public:
    void findUperf();
    bool isUperf();
    void start();
    void stop();
    int getPid() const;
};