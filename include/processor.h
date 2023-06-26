#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  long prevuser, prevnice, prevsystem, previdle, previowait, previrq,
      prevsoftirq, prevsteal;
  float cpu_percent;
  // TODO: Declare any necessary private members
 private:
};

#endif