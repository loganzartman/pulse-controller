#pragma once

namespace Stats {
  const int REPORT_INTV = 100;
  const int N_KEYS = 1;
  const int KEY_STEPPER = 0;
  const int KEY_LED = 1;
  const String KEY_NAMES[] = {
    "steppers",
    "led",
  };
  
  unsigned long micros_start[N_KEYS];
  unsigned long sample_micros_sum[N_KEYS];
  int sample_count[N_KEYS];

  void start(int key) {
    micros_start[key] = micros();
  }
  
  void finish(int key) {
    ++sample_count[key];
    sample_micros_sum[key] += micros() - micros_start[key];
    if (sample_count[key] >= REPORT_INTV) {
      unsigned long avg_micros = sample_micros_sum[key] / sample_count[key];
      sample_count[key] = 0;
      sample_micros_sum[key] = 0;
      Serial.print(KEY_NAMES[key]);
      Serial.print(": ");
      Serial.println(avg_micros);
    }
  }
}
