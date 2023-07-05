#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};
const std::string osName("PRETTY_NAME");
const std::string memTotalKey("MemTotal");
const std::string memFreeKey("MemFree");
const std::string processes("processes");
const std::string procsRunning("procs_running");
//exact physical memory as opposed to virtual memory size (VmSize)
const std::string vmRss("VmRSS:");
const std::string uid("Uid:");


float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

template <typename T>
T findValueByKey(std::string const &fileName, std::string const &keyName) {
  std::string line, key;
  T value;
  std::ifstream stream(kProcDirectory + fileName);
  if(stream.is_open()) {
    while(std::getline(stream, line)){
     if(line.find(keyName) != std::string::npos){
      std::istringstream linestream(line);
      linestream >> key >> value;
      return value;
     }
    }
  }
  return value;
};

}; // namespace LinuxParser

#endif