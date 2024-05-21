#include "file_tape_device.h"

FileTapeDevice::FileTapeDevice(Delays delays, int64_t size, int64_t id)
    : delays_(delays), count_numbers_(size) {
  std::string directory = "tmp";
  if (!std::filesystem::exists(directory)) {
    if (!std::filesystem::create_directory(directory)) {
      std::cout << "Failed to create directory: " << directory << std::endl;
      exit(1);
    }
  }
  file_name_ = directory + "tmp_tape" + std::to_string(id) + ".txt";
  std::ofstream file;
  file.open(file_name_);
  std::cout << 1;
  if (!file.is_open()) {
    std::cout << "Could 2not open the file " << file_name_ << std::endl;
    exit(1);
  }
  for (int i = 0; i < size; ++i) {
    std::streampos pos = i * 11;
    file << "          \n";
  }
  file.close();
  file_.open(file_name_, std::ios::in | std::ios::out);
  if (!file_.is_open()) {
    std::cout << "Could 1not open the file " << file_name_ << std::endl;
    exit(1);
  }
}

FileTapeDevice::FileTapeDevice(std::string &file_name, Delays delays)
    : file_name_(file_name), delays_(delays) {
  file_.open(file_name_, std::ios::in | std::ios::out);
  if (!file_.is_open()) {
    std::cout << "Could 1not open the file " << file_name_ << std::endl;
    exit(1);
  }

  file_.seekg(0, std::ios::end);
  end_pos_ = file_.tellg();
  file_.seekg(0, std::ios::beg);
  start_pos_ = file_.tellg();
  count_numbers_ = end_pos_ / 11;
  count_numbers_ = 3;
}

void FileTapeDevice::CreateTape(int64_t size) {
  count_numbers_ = size;
  for (int i = 0; i < count_numbers_; ++i) {
    std::streampos pos = i * 11;
    char d[] = "          \n";
    char *data = d;
    file_.write(data, 10);
  }
  file_.seekg(0, std::ios::end);
  end_pos_ = file_.tellg();
  file_.seekg(0, std::ios::beg);
}

int FileTapeDevice::Read() {
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delays_.reading_delay_));
  int res = 0;
  std::streampos pos = current_number_ * 11;
  // file_.read((char*)&res, 10);
  std::string str;
  file_ >> str;
  return std::stoi(str);
}

void FileTapeDevice::Write(int data) {
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delays_.writing_delay_));
  std::streampos pos = current_number_ * 11;
  file_ << data;
  file_ << '\n';
  // file_.write((char*)&data, 10);
}

void FileTapeDevice::RightShift() {
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_.shift_delay_));
  if (current_number_ == count_numbers_) {
    std::cout << "It is impossible to move to the right" << std::endl;
    return;
  }
  ++current_number_;
}
void FileTapeDevice::LeftShift() {
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_.shift_delay_));
  if (current_number_ == 0) {
    std::cout << "It is impossible to move to the left" << std::endl;
    return;
  }
  --current_number_;
}
bool FileTapeDevice::IfEnd() { return current_number_ == count_numbers_; };
void FileTapeDevice::RewindToBeginning() {
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_.rewind_delay_));
  current_number_ = 0;
}
void FileTapeDevice::RewindToEnd() {
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_.rewind_delay_));
  current_number_ = count_numbers_;
}

FileTapeDevice::~FileTapeDevice() {
  if (file_.is_open()) {
    file_.close();
  }
}
