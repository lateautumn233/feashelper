#include <signal.h>
#include <fstream>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/prctl.h>
#include "include/Uperfhelper.h"

#define SIGSTOP 19
#define SIGCONT 18

static void Uperfhelper(bool &stat, bool &stop)
{
    prctl(PR_SET_NAME, "Uperfhelper");
    Uperf theUperf;
    stat = false;
    while (true)
    {
        theUperf.findUperf();
        sleep(10);
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
            usleep(10000);
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


void startUperfhelper(bool &stat, bool &stop) // a thread for watching uperf
{
    std::thread uperfhelper(Uperfhelper, std::ref(stat), std::ref(stop));
    uperfhelper.detach();
}

void Uperf::findUperf()
{
    static std::ifstream comm;
    static std::string location, name;
    uperf_pid = -1; // reset pid
    static int pid1, pid2; // uperf has two daemon
    comm.open(location.c_str());
    static bool uperfn = false;
    DIR *Proc;
    static struct dirent *direntp;
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
        } // finded 1st pid
        if (name == "uperf" && uperfn)
        {
            pid2 = atoi(direntp->d_name); // finded 2nd pid
            uperf_pid = std::max(pid1, pid2); // the later one
            std::cout << "uperf pid = " << uperf_pid << '\n';
            break;
        }
    }
    closedir(Proc);
}

bool Uperf::isUperf() // if not, findUperf again
{
    static std::string location = "/proc/" + std::to_string(uperf_pid) + "/comm", name;
    static std::ifstream comm(location.c_str());
    if (!comm)
        return false;
    comm >> name;
    comm.close();
    return (name == "uperf");
}

void Uperf::start() // resume uperf from freezed
{
    kill(uperf_pid, SIGCONT);
}

void Uperf::stop() // freeze uperf
{
    kill(uperf_pid, SIGSTOP);
}

int Uperf::getPid() const
{
    return uperf_pid;
}