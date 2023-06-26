#include "format.h"

#include <cmath>
#include <iostream>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hour, min, sec;
  string hstring, mstring, sstring;

  hour = floor(seconds / 3600);
  min = floor((seconds - (hour * 3600)) / 60);
  sec = seconds - (hour * 3600) - (min * 60);

  if (hour < 10) {
    hstring = "0" + std::to_string(hour);
  } else
    hstring = std::to_string(hour);

  if (min < 10) {
    mstring = "0" + std::to_string(min);
  } else
    mstring = std::to_string(min);

  if (sec < 10) {
    sstring = "0" + std::to_string(sec);
  } else
    sstring = std::to_string(sec);

  return hstring + ":" + mstring + ":" + sstring;
}