#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  // LinuxParser::Pids()
  // process: pid, user, cpu, ram, uptime, command
  // container composed of cpu, user, pid, RAM, and uptime
  vector<int> process_ids = LinuxParser::Pids();
  for (int &i : process_ids) {
    Process newprocesses(i);
    processes_.emplace_back(newprocesses);
  }
  // int k = 0;
  // for (int j = 0; j < process_ids.size(); ++j){
  //   float val = processes_[1].getCpu();
  //   int index = j;
  //   while(index >0 && processes_[index-1].getCpu()){
  //     processes_[index] = processes_[index-1];
  //   }
  //   if(processes_[j].getCpu() < processes_[k].getCpu()){
      
  //   }
  // }
 
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  // inside /proc/meminfo
  // MemTotal, MemFree, MemAvailable, and Buffers

  return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }