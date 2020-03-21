#ifndef DEMO_LIB_GUARDIAN_H
#define DEMO_LIB_GUARDIAN_H

class Singleton {
public:
  //синглтон Майерса
  static Singleton *instance() {
    static Singleton inst;
    return &inst;
  }

private:
  Singleton();                                  // Private constructor
  ~Singleton() = default;
  Singleton(const Singleton &);                 // Prevent copy-construction
  Singleton &operator=(const Singleton &);      // Prevent assignment
};

#define N 8
uint32_t ema(uint32_t new_sample)
{
    static uint32_t samples[N];
    static uint32_t total;
    static uint8_t i = 0;

    total -= samples[i];
    total += new_sample;
    samples[i] = new_sample;
    i = (i+1) % N;
    uint32_t average = total >> 3;
    return average;
}

#define N 8
uint32_t ema(uint32_t new_sample) {
  static uint32_t samples[N];
  static uint32_t total;
  static uint8_t i = 0;

  total -= samples[i];
  total += new_sample;
  samples[i] = new_sample;
  i = (i + 1) % N;
  uint32_t average = total >> 3;
  return average;
}

#endif //DEMO_GUARDIAN_H