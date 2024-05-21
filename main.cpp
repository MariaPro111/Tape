#include "file_tape_device.h"
#include "tape_sort.h"
#include <iostream>

int GetConfigParameters(std::string &line) {
  std::string str;
  bool f = false;
  for (int i = 0; i < line.size(); ++i) {
    if (line[i] == '=') {
      f = true;
    } else if (f) {
      str += line[i];
    }
  }
  return std::stoi(str);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Input data error. Please add input and output files."
              << std::endl;
    return 1;
  }
  int64_t M = 0;
  Delays delays;
  if (argc == 4) {
    std::string config_name = argv[3];
    std::ifstream file(config_name);
    if (!file.is_open()) {
      std::cout << "Failed to open config file " << config_name << std::endl;
      return 1;
    }
    std::string line;
    std::getline(file, line);
    delays.writing_delay_ = GetConfigParameters(line);
    std::getline(file, line);
    delays.reading_delay_ = GetConfigParameters(line);
    std::getline(file, line);
    delays.rewind_delay_ = GetConfigParameters(line);
    std::getline(file, line);
    delays.shift_delay_ = GetConfigParameters(line);
    std::getline(file, line);
    M = GetConfigParameters(line);
  }
  std::string input_filename = argv[1];
  std::string output_filename = argv[2];
  TapeSort sorter(input_filename, output_filename, delays, M);
  sorter.UploadingData();
  return 0;
}
