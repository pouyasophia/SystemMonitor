#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

#include "format.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream MUFileStream(kProcDirectory + kMeminfoFilename);
  string line, name;
  float value, memtotal, memfree;
  if (MUFileStream.is_open()) {
    while (std::getline(MUFileStream, line)) {
      if (line.find("MemTotal") != string::npos) {
        std::istringstream MULineStream(line);
        MULineStream >> name >> value;
        memtotal = value;
      }
      if (line.find("MemFree") != string::npos) {
        std::istringstream MULineStream(line);
        MULineStream >> name >> value;
        memfree = value;
      }
    }
  }
  return (memtotal - memfree) / memtotal;
}

long LinuxParser::UpTime() {
  std::ifstream UTFileStream;
  UTFileStream.open(kProcDirectory + kUptimeFilename);

  long time_system, time_idle;
  UTFileStream >> time_system >> time_idle;

  return time_system;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() {
  std::ifstream TPFileStream(kProcDirectory + kStatFilename);
  string line, key;
  int value;

  if (TPFileStream.is_open()) {
    while (std::getline(TPFileStream, line)) {
      if (line.find("processes") != string::npos) {
        std::istringstream TPLineStream(line);
        TPLineStream >> key >> value;
        return value;
        break;
      }
    }
  } return value;
}

int LinuxParser::RunningProcesses() {
  std::ifstream PRFileStream(kProcDirectory + kStatFilename);
  string line, key;
  int value;

  if (PRFileStream.is_open()) {
    while (std::getline(PRFileStream, line)) {
      if (line.find("procs_running") != string::npos) {
        std::istringstream PRLineStream(line);
        PRLineStream >> key >> value;
        return value;
        break;
      }
    }
  } return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string command;
  std::ifstream commandfilestream(kProcDirectory + to_string(pid) + "/" +
                                  kCmdlineFilename);
  if (commandfilestream.is_open()) {
    std::getline(commandfilestream, command);
    return command;
  }
  commandfilestream.close();

  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string mem_used, line, mem, unit;
  std::ifstream memfilestream(kProcDirectory + to_string(pid) + "/" +
                              kStatusFilename);
  if (memfilestream.is_open())
    while (std::getline(memfilestream, line)) {
      if (line.find("VmSize:") != string::npos) {
        std::istringstream memlinestream(line);
        memlinestream >> mem >> mem_used;
        break;
      }
    }
  return mem_used;
}

string LinuxParser::Uid(int pid) {
  std::string uid, uid_number, line;
  std::ifstream uidfilestream(kProcDirectory + to_string(pid) + "/" +
                              kStatusFilename);
  if (uidfilestream.is_open())
    while (std::getline(uidfilestream, line)) {
      if (line.find("Uid:") != string::npos) {
        std::istringstream uidlinestream(line);
        uidlinestream >> uid >> uid_number;
        break;
      }
    }
  return uid_number;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
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
      for (int i = 0; i < line.length(); ++i) {
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

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream uptimestream(kProcDirectory + std::to_string(pid) + "/" +
                             kStatFilename);
  std::string line, ans_str;
  long clockticks, time_sec;
  bool exit = false;
  int j = 0;
  while (std::getline(uptimestream, line) && !exit) {
    for (int i = 0; i < line.length(); ++i) {
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
  // Format::ElapsedTime?? should I return string here?
}
