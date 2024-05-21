#include "tape_sort.h"

#include <vector>

TapeSort::TapeSort(std::string &file_in, std::string &file_out, Delays delays,
                   int64_t M)
    : input_tape_(file_in, delays), output_tape_(file_out, delays),
      memory_size_(M) {
  tape_size_ = input_tape_.count_numbers_;
  output_tape_.CreateTape(tape_size_);
}

void TapeSort::UploadingData() {
  while (!input_tape_.IfEnd()) {
    for (int64_t i = 0; i < memory_size_; ++i) {
      if (input_tape_.IfEnd()) {
        break;
      }
      tape_part_.push_back(input_tape_.Read());
      input_tape_.RightShift();
    }
    Sort();
    FileTapeDevice tmp_tape(input_tape_.delays_, tape_part_.size(), id_);
    for (auto x : tape_part_) {
      tmp_tape.Write(x);
      tmp_tape.RightShift();
    }
    tmp_tapes_.push_back(tmp_tape.file_name_);
    tmp_tape.file_.close();
    ++id_;
    tape_part_.clear();
  }
  Merge();
  Merge();
}

void TapeSort::Merge() {
  std::vector<std::string> tmp_tapes = tmp_tapes_;
  tmp_tapes_.clear();
  ++id_;
  for (int i = 1; i < tmp_tapes.size(); i + 2) {
    FileTapeDevice f1(tmp_tapes[i - 1], input_tape_.delays_);
    FileTapeDevice f2(tmp_tapes[i], input_tape_.delays_);
    FileTapeDevice merge(input_tape_.delays_,
                         f1.count_numbers_ + f2.count_numbers_, id_);
    ++id_;
    int k1 = 0;
    int k2 = 0;
    int f = 0;
    while (!f1.IfEnd() && !f2.IfEnd()) {
      int a1;
      int a2;
      if (f == 0) {
        a1 = f1.Read();
        a2 = f2.Read();
        f1.RightShift();
        f2.RightShift();
      } else if (f == 1) {
        a1 = f1.Read();
        a2 = k2;
        f1.RightShift();
      } else {
        a1 = k1;
        a2 = f2.Read();
        f2.RightShift();
      }

      if (a1 <= a2) {
        merge.Write(a1);
        merge.RightShift();
        f = 1;
        k2 = a2;
      } else {
        merge.Write(a2);
        merge.RightShift();
        f = 2;
        k1 = a1;
      }
    }
    if (!f1.IfEnd()) {
      while (!f1.IfEnd()) {
        merge.Write(f1.Read());
        f1.RightShift();
        merge.RightShift();
      }
    }
    if (!f2.IfEnd()) {
      while (!f2.IfEnd()) {
        merge.Write(f2.Read());
        f2.RightShift();
        merge.RightShift();
      }
    }
    merge.file_.close();
    tmp_tapes_.push_back(merge.file_name_);
  }
  if (tmp_tapes.size() % 2 != 0) {
    tmp_tapes_.push_back(tmp_tapes[tmp_tapes.size() - 1]);
  }
}

void TapeSort::QuickSort(int *mas, int64_t size) {
  int64_t i = 0;
  int64_t j = size - 1;
  int mid = mas[size / 2];
  do {
    while (mas[i] < mid) {
      i++;
    }
    while (mas[j] > mid) {
      j--;
    }
    if (i <= j) {
      int tmp = mas[i];
      mas[i] = mas[j];
      mas[j] = tmp;
      i++;
      j--;
    }
  } while (i <= j);
  if (j > 0) {
    QuickSort(mas, j + 1);
  }
  if (i < size) {
    QuickSort(&mas[i], size - i);
  }
}
void TapeSort::Sort() {
  int *mas[tape_part_.size()];
  int i = 0;
  for (auto &x : tape_part_) {
    std::cout << x << ' ';
    mas[i] = &x;
    ++i;
  }
  std::cout << '\n';
  QuickSort(*mas, tape_part_.size());
  i = 0;
  for (auto &x : tape_part_) {
    x = *mas[i];
    std::cout << x << ' ';
    ++i;
  }
  std::cout << '\n';
}
