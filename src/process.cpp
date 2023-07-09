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

Process::Process(int pid) {
  pid_ = pid;
  user_ = LinuxParser::User(pid_);
  cpu_ = CpuUtilization();
  ram_ = LinuxParser::Ram(pid_);
  uptime_ = Process::UpTime();
  command_ = LinuxParser::Command(pid_);
}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() {
  float cpu_percent;

  std::ifstream uptimestream(LinuxParser::kProcDirectory +
                             std::to_string(Process::Pid()) + "/" +
                             LinuxParser::kStatFilename);
  std::string line;

  float utime, stime, cutime, cstime, starttime;
  bool exit = false;
  float sys_uptime = LinuxParser::UpTime();

  int j = 0;
  while (std::getline(uptimestream, line, ' ') && !exit) {
    std::istringstream UTlinestream(line);
    ++j;
    if (j == 14) {
      utime = stof(line);
    }
    if (j == 15) {
      stime = stof(line);
    }
    if (j == 16) {
      cutime = stof(line);
    }
    if (j == 17) {
      cstime = stof(line);
    }
    if (j == 22) {
      starttime = stof(line);
    }
  }

  float total_time = utime + stime + cutime + cstime;
  float seconds = sys_uptime - (starttime / (sysconf(_SC_CLK_TCK)));
  uptime_ = seconds;
  cpu_percent = ((total_time / (sysconf(_SC_CLK_TCK))) / seconds);
  return cpu_percent;
}

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() 
{ 
  return uptime_; 
}

bool Process::operator<(Process const &a) const {
  return (stol(ram_) > stol(a.ram_));
}
