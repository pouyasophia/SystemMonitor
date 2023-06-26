#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // define previous value totals
  long previdletotal = previdle + previowait;
  long prevnonidle =
      prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  long prevtotal = previdle + prevnonidle;

  // open proc/filename to, extract current values, and calculate memory
  // utilization
  std::ifstream cpufilestream;
  cpufilestream.open(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string cpu;
  cpufilestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
      softirq >> steal >> guest >> guest_nice;
  cpufilestream.close();
  long idletotal = idle + iowait;
  long nonidle = user + nice + system + irq + softirq + steal;
  long total = idletotal + nonidle;
  long totald = total - prevtotal;
  long idled = idletotal - previdletotal;
  cpu_percent = (totald - idled) / totald;

  // set previous value to current value for next time the function is called
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