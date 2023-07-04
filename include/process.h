#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
  Process(int pid);
  int Pid() const;
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const &a) const;

  float getCpu() { return cpu_; }

private:
  int pid_;
  std::string user_;
  float cpu_;
  std::string ram_;
  long uptime_;
  std::string command_;
};

#endif