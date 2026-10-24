#include "fft.h"
#include <algorithm>

fft::fft() {
  raw_magnitude_values = new float[NUM_BINS]();
  prev_mag_values = new float[NUM_BINS]();
  magnitude_values.store(new float[NUM_BINS]());
  // initializing magnitude values is important.
  // can't init to nullptr since on second call of audio processor it will write
  // into nullptr.
  // so we store the pointer of a newly allocated array into
  // atomic so we could then swap them around
  input_ = fftwf_alloc_real(BUFFER_SIZE);
  out_ = fftwf_alloc_complex(NUM_BINS);
  plan_ = fftwf_plan_dft_r2c_1d(BUFFER_SIZE, input_, out_, FFTW_ESTIMATE);
}

fft::~fft() {
  delete[] raw_magnitude_values;
  delete[] magnitude_values.load();
  fftwf_destroy_plan(plan_);
  fftwf_free(input_);
  fftwf_free(out_);
}

void fft::apply_fft_on_window(const float *samples) {
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    double window = hann(i);
    // aplying hanning window technique to avoid spectral leakage.

    // taking only left channel. Samples contain value of L R L R...
    // buffer size in this case is = 512, samples contains 512 frames but
    // expands to 1024 values for 2 channels.
    input_[i] = samples[i * 2] * window;
  }

  fftwf_execute(plan_);

  // std::vector<float> bins_magnitude;
  for (int k = 0; k < NUM_BINS; k++) {
    float real = out_[k][0];
    float imag = out_[k][1];
    float magnitude = std::sqrt(real * real + imag * imag);
    // Convert to dBFS(add small epsilon to avoid log(0))
    // converting to decibels smoothes out the distribution of values so
    // the difference is not that huge.
    float dB = 20.0f * std::log10f(magnitude / BUFFER_SIZE + 1e-12);
    // Max (loudest): 0 dB — when magnitude / BUFFER_SIZE = 1.0
    // Min (weakest): 20 * log10(1e-12) ≈ -240 dB — pure silence

    // most real signals live somewhere between -80 and 0.
    float normalized = (dB - DB_MIN) / (DB_MAX - DB_MIN);
    // normalizing the values to 0..1 range
    raw_magnitude_values[k] = std::clamp(normalized, 0.0f, 1.0f);
  }
  raw_magnitude_values = magnitude_values.exchange(raw_magnitude_values);
}
