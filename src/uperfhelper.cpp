#include <signal.h>
#include <fstream>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
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

static int is_pid_folder(const struct dirent *entry) {
    const char *p;
 
    for (p = entry->d_name; *p; p++) {
        if (!isdigit(*p))
            return 0;
    }
    return 1;
}


void startUperfhelper(bool &stat, bool &stop)
{
    std::thread uperfhelper(Uperfhelper, std::ref(stat), std::ref(stop));
    uperfhelper.detach();
}

void Uperf::findUperf()
{
    std::ifstream comm;
    std::string location, name;
    uperf_pid = -1;
    int pid1, pid2;
    comm.open(location.c_str());
    bool uperfn = false;
    DIR *Proc;
    struct dirent *direntp;
    Proc = opendir("/proc");
    while((direntp = readdir(Proc)))
    {
        if (!is_pid_folder(direntp))
            continue;
        location = "/proc/" + std::string(direntp->d_name) + "/comm";
        comm.open(location.c_str());
        if (!comm)
        {
            comm.close();
            continue;
        }
        comm >> name;
        comm.close();
        if (name == "uperf" && !uperfn)
        {
            uperfn = true;
            pid1 = atoi(direntp->d_name);
            continue;
        }
        if (name == "uperf" && uperfn)
        {
            pid2 = atoi(direntp->d_name);
            uperf_pid = std::max(pid1, pid2);
            std::cout << "uperf pid = " << uperf_pid << '\n';
            break;
        }
    }
    closedir(Proc);
}

bool Uperf::isUperf()
{
    std::string location = "/proc/" + std::to_string(uperf_pid) + "/comm", name;
    std::ifstream comm(location.c_str());
    if (!comm)
        return false;
    comm >> name;
    comm.close();
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
    
}