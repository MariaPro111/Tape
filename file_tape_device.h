#pragma once

#include "tape_device.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

struct Delays {
  int writing_delay_ = 0;
  int reading_delay_ = 0;
  int rewind_delay_ = 0;
  int shift_delay_ = 0;
};

class FileTapeDevice : public TapeDevice<int> {
public:
  Delays delays_;
  std::string file_name_;
  std::fstream file_;
  std::ofstream tmp_file_;
  std::streampos start_pos_;
  std::streampos end_pos_;
  int64_t count_numbers_;
  int64_t current_number_ = 0;

  FileTapeDevice(Delays delays, int64_t size, int64_t id);

  FileTapeDevice(std::string &file_name, Delays delays);

  void CreateTape(int64_t size);

  int Read() override;

  void Write(int data) override;

  void RightShift() override;
  void LeftShift() override;
  bool IfEnd() override;
  void RewindToBeginning() override;
  void RewindToEnd() override;

  ~FileTapeDevice() override;
};