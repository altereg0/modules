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
#endif //DEMO_GUARDIAN_H