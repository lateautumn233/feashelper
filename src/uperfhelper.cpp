#include <signal.h>
#include <fstream>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/prctl.h>
#include "include/uperfhelper.h"

#define SIGSTOP 19
#define SIGCONT 18

static void Uperfhelper(bool &stat, bool &stop)
{
    prctl(PR_SET_NAME, "Uperfhelper");
    Uperf theUperf;
    stat = false;
    while (true)
    {
        sleep(10);
        theUperf.findUperf();
        if (theUperf.getPid() == -1)
        {
            stat = false;
            continue;
        }
        stat = true;
        while(theUperf.isUperf())
        {
            if (!stop)
                theUperf.start();
            else
                theUperf.stop();
            sleep(1);
        }
    }
}

void startUperfhelper(bool &stat, bool &stop)
{
    std::thread uperfhelper(Uperfhelper, std::ref(stat), std::ref(stop));
    uperfhelper.detach();
}

void Uperf::findUperf()
{
    std::ifstream comm;
    std::string location("/proc/1/comm"), name;
    uperf_pid = -1;
    comm.open(location.c_str());
    bool uperfn = false;
    for (int pid = 1; pid < 9999999; pid++)
    {
        comm >> name;
        if (name == "uperf")
            uperfn = true;
        if (name == "uperf" && uperfn)
        {
            uperf_pid = pid;
            break;
        }
        comm.close();
        comm.open(location.c_str());
        location = "/proc/" + std::to_string(pid) + "/comm";
    }
}

bool Uperf::isUperf()
{
    std::string location = "/proc/" + std::to_string(uperf_pid) + "/comm", name;
    std::ifstream comm(location.c_str());
    if (!comm)
        return false;
    comm >> name;
    return (name == "uperf");
}

void Uperf::start()
{
    if (stopped)
        kill(uperf_pid, SIGCONT);
}

void Uperf::stop()
{
    kill(uperf_pid, SIGSTOP);
    stopped = true;
}

int Uperf::getPid() const
{
    return uperf_pid;
}

Uperf::Uperf()
{
    findUperf();
}