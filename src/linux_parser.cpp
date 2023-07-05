#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == osName) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
};

float LinuxParser::MemoryUtilization() {

  float memTotal = findValueByKey<float>(kMeminfoFilename, memTotalKey);
  float memFree = findValueByKey<float>(kMeminfoFilename, memFreeKey);
  return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime() {
  std::ifstream UTFileStream;
  UTFileStream.open(kProcDirectory + kUptimeFilename);

  long time_system, time_idle;
  UTFileStream >> time_system >> time_idle;

  return time_system;
}

int LinuxParser::TotalProcesses() {
  int totalProcesses = findValueByKey<int>(kStatFilename, processes);
  return totalProcesses;
}

int LinuxParser::RunningProcesses() {

  int runningNumber = findValueByKey<int>(kStatFilename, procsRunning);
  return runningNumber;
}

string LinuxParser::Command(int pid) {
  std::string command;
  std::ifstream commandfilestream(kProcDirectory + to_string(pid) + "/" +
                                  kCmdlineFilename);
  if (commandfilestream.is_open()) {
    std::getline(commandfilestream, command);
    if (command.length() > 40){
      command.erase(command.begin() + 40,command.end());
      command.append("...");
    }
    return command;
  }
  commandfilestream.close();
  return command;
}

string LinuxParser::Ram(int pid) {
  std::string memUsed = "0";
  long tempMem = findValueByKey<long>(to_string(pid) + "/" + kStatusFilename, vmRss);
  if (tempMem > 0){
    memUsed = to_string(tempMem/1000);
  };
  return memUsed;
  
}

string LinuxParser::Uid(int pid) {
  
  std::string uidNumber = findValueByKey<std::string>(to_string(pid) + "/" + kStatusFilename, uid);
  return uidNumber;
}

string LinuxParser::User(int pid) {
  std::string ref;
  ref = LinuxParser::Uid(pid);
  std::string uname_correct, line;
  std::ifstream userfilestream(kPasswordPath);
  bool exit = false;

  if (userfilestream.is_open()) {
    while (std::getline(userfilestream, line) && !exit) {
      std::string user_name, x, uid;

      int j = 0;
      for (unsigned int i = 0; i < line.length(); ++i) {
        if (line[i] == ':') {
          j++;
          i++;
        }
        if (j == 0) {
          user_name.push_back(line[i]);
        }
        if (j == 1) {
          x.push_back(line[i]);
        }
        if (j == 2) {
          uid.push_back(line[i]);
        }
        if (j > 4) {
          break;
        }
        if (j == 2 && uid == ref) {
          uname_correct = user_name;
          exit = true;
          break;
        }
      }
    }
  }
  return uname_correct;
}

long LinuxParser::UpTime(int pid) {
  std::ifstream uptimestream(kProcDirectory + std::to_string(pid) + "/" +
                             kStatFilename);
  std::string line, ans_str;
  long clockticks, time_sec;
  bool exit = false;
  int j = 0;
  while (std::getline(uptimestream, line) && !exit) {
    for (unsigned int i = 0; i < line.length(); ++i) {
      if (line[i] == ' ') {
        ++j;
      }
      if (j == 21) {
        ans_str.push_back(line[i]);
      }
      if (j == 22) {
        clockticks = stol(ans_str);
        exit = true;
      }
    }
  }
  time_sec = clockticks / sysconf(_SC_CLK_TCK);
  return time_sec;
}
