#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
  long previdletotal = previdle + previowait;
  long prevnonidle =
      prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  long prevtotal = previdle + prevnonidle;

  std::ifstream cpufilestream;
  cpufilestream.open(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string cpu, line;
  getline(cpufilestream, line);
  std::istringstream cpulinestream(line);
  cpulinestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
      softirq >> steal >> guest >> guest_nice;
  cpufilestream.close();
  float idletotal = idle + iowait;
  float nonidle = user + nice + system + irq + softirq + steal;
  float total = idletotal + nonidle;
  float totald = total - prevtotal;
  float idled = idletotal - previdletotal;
  cpu_percent = (totald - idled) / totald;

  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previdle = idle;
  previowait = iowait;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;

  return cpu_percent;
}