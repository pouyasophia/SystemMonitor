#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) {
  pid_ = pid;
  user_ = LinuxParser::User(pid_);
  cpu_ = CpuUtilization();
  ram_ = LinuxParser::Ram(pid_);
  uptime_ = LinuxParser::UpTime(pid_);
  command_ = LinuxParser::Command(pid_);
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    float cpu_percent;
  
  std::ifstream uptimestream(LinuxParser::kProcDirectory + std::to_string(Process::Pid()) + "/" +
                             LinuxParser::kStatFilename);
  std::string line;
  
  float utime, stime, cutime, cstime, starttime; 
  bool exit = false;
  float sys_uptime = LinuxParser::UpTime();
  
  int j = 0;
  while (std::getline(uptimestream, line, ' ') && !exit) {
    std::istringstream UTlinestream(line);
    ++j;
    if(j == 14){
      utime = stof(line);
    }
    if(j == 15){
      stime = stof(line);
    }
    if(j == 16) {
      cutime = stof(line);
    }
    if(j == 17){
      cstime = stof(line);
    }
    if(j == 22){
      starttime = stof(line);
    }
  }
  //clock ticks

  float total_time = utime + stime + cutime + cstime;
  float seconds = sys_uptime - (starttime / (sysconf(_SC_CLK_TCK)));
  cpu_percent = 100 * ((total_time/(sysconf(_SC_CLK_TCK)))/seconds);
  return cpu_percent;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// sort by most ram or most cpu utilization
bool Process::operator<(Process const& a) const {
  //this is supposed to be the sorting operator but idk how to use it
  if (cpu_ < a.cpu_) {
    return true;
  } else {
    return false;
  };
}
  