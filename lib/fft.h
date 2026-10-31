#include <atomic>
#include <fftw3.h>
#include <math.h>

constexpr int SAMPLE_RATE = 48000;
constexpr int BUFFER_SIZE = 512;
constexpr int BIN_FREQUENCY = SAMPLE_RATE / BUFFER_SIZE;
constexpr int TOTAL_BINS = BUFFER_SIZE / 2 + 1;
constexpr int DISPLAY_BINS = BUFFER_SIZE / 4 + 1;
constexpr int DB_MAX = 0;
constexpr int DB_MIN = -80;

class fft {
public:
  fft();
  ~fft();

  void apply_fft_on_window(const float *window);
  std::atomic<float *> magnitude_values;
  float *prev_mag_values;

private:
  float *input_;
  fftwf_complex *out_;
  fftwf_plan plan_;
  float *raw_magnitude_values;

  static double hann(int i) {
    return 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (BUFFER_SIZE - 1)));
  }
};
