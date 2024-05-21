#pragma once

#include "file_tape_device.h"
#include <vector>

class TapeSort {
public:
  FileTapeDevice input_tape_;
  FileTapeDevice output_tape_;
  int64_t memory_size_;
  int64_t tape_size_;
  int64_t tmp_tape_size_;
  std::vector<int> tape_part_;
  int64_t current = 0;
  std::vector<std::string> tmp_tapes_;
  int64_t id_ = 5;
  TapeSort(std::string &file_in, std::string &file_out, Delays delays,
           int64_t M);

  void UploadingData();

  void Merge();

  void QuickSort(int *mas, int64_t size);
  void Sort();
};
