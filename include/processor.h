#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  float Utilization();
  float user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  float prevuser, prevnice, prevsystem, previdle, previowait, previrq,
      prevsoftirq, prevsteal;
  float cpu_percent;
};

#endif