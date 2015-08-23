#include <sys/resource.h>
#include <thread>
#include <iostream>
#include <list>
#include <mutex>
#include <string>
#include <iomanip>
#include "MemoryPool.hpp"

#if DM_PLATFORM_LINUX
#include <fstream>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

double initialMemory;
double currentMemory;
double residentSet;

#endif

class A {
public:
  A(int id_) : ID(id_) {};
  int ID;
  char foo [1000000];
};

std::mutex mt;
std::list<A*> objectsList;
volatile bool exitCondition = false;
dinahmoe::utils::MemoryPool <A> mp;
int currentId = 0;

void produce() {
  while (!exitCondition) {
    mt.lock();
    objectsList.emplace_back(mp.acquire(currentId++));
    mt.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void consume() {
  while (!exitCondition) {
    mt.lock();
    for (auto item: objectsList) {
      mp.release(item);
    }
    objectsList.clear();
    std::cout << "Clearing memory, live bytes shall be zero now" << std::endl << std::endl;
    mt.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void monitor() {
#if DM_PLATFORM_LINUX
  process_mem_usage(initialMemory,residentSet);
  std::cout << "Initial memory " << initialMemory << std::endl;
#endif
  std::cout << std::fixed;
  std::cout << std::setprecision(2);
  while (!exitCondition) {
    mt.lock();
#if DM_PLATFORM_LINUX
    process_mem_usage(currentMemory,residentSet);
    std::cout << (currentMemory - initialMemory) << " " << residentSet << std::endl;
#else
    int who = RUSAGE_SELF;
    struct rusage usage;
    int ret;
    ret=getrusage(who,&usage);
//    std::cout << usage.ru_utime.tv_sec << " " << usage.ru_utime.tv_usec << " user time used" << std::endl;
//    std::cout << usage.ru_stime.tv_sec << " " << usage.ru_utime.tv_usec << " system time used" << std::endl;
    std::cout << (float)usage.ru_maxrss / 1000000.F << " MB max resident set size" << std::endl << std::endl;
//    std::cout << usage.ru_ixrss << " integral shared text memory size" << std::endl;
//    std::cout << usage.ru_idrss << " integral unshared data size" << std::endl;
//    std::cout << usage.ru_isrss << " integral unshared stack size" << std::endl;
//    std::cout << usage.ru_minflt << " page reclaims" << std::endl;
//    std::cout << usage.ru_majflt << " page faults" << std::endl;
//    std::cout << usage.ru_nswap << "  swaps" << std::endl;
//    std::cout << usage.ru_inblock << " block input operations" << std::endl;
//    std::cout << usage.ru_oublock << " block output operations" << std::endl;
//    std::cout << usage.ru_msgsnd << " messages sent" << std::endl;
//    std::cout << usage.ru_msgrcv << " messages received" << std::endl;
//    std::cout << usage.ru_nsignals << " signals received" << std::endl;
//    std::cout << usage.ru_nvcsw << "  voluntary context switches" << std::endl;
//    std::cout << usage.ru_nivcsw << " involuntary context switches" << std::endl << std::endl;
#endif
    mt.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {
  std::thread t1(produce);
  std::thread t2(consume);
  std::thread t3(monitor);
  std::string line;
  std::getline (std::cin, line);
  t3.join();
  t2.join();
  t1.join();
}
