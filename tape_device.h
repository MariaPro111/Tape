#pragma once

template <typename T> class TapeDevice {
public:
  virtual ~TapeDevice() = default;
  virtual void Write(T data) = 0;
  virtual T Read() = 0;
  virtual void RightShift() = 0;
  virtual void LeftShift() = 0;
  virtual bool IfEnd() = 0;
  virtual void RewindToBeginning() = 0;
  virtual void RewindToEnd() = 0;
};
