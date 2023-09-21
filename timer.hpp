#pragma once

#include <chrono>

class timer {
  typedef std::chrono::high_resolution_clock::time_point time_point;
  typedef std::chrono::duration<double>                  duration_type;

public:
  void start() { then = std::chrono::high_resolution_clock::now(); }
  void stop() { now = std::chrono::high_resolution_clock::now(); }
  double elapsed() { return std::chrono::duration_cast<duration_type>(now - then).count(); }

private:
  time_point then, now;
};

template < typename callable >
double runtime(callable f) {
  timer stopwatch;
  stopwatch.start();
  f();
  stopwatch.stop();
  return stopwatch.elapsed();
}