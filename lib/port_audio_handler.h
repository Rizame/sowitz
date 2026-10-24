#include "fft.h"
#include "pulse_audio_handler.h"
#include <portaudio.h>
#include <string>

#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER (512)

struct DefaultDeviceInfo {
  std::string sink_monitor;
  int id;
  double suggested_latency;
};

class port_audio_handler {
private:
  PaError err;
  PaStream *stream;
  pulse_audio_handler pulse_audio_h;
  //   PaStreamParameters input;

  [[nodiscard]] DefaultDeviceInfo initialize_default_device();

  static int pa_callback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags, void *userData);
  void checkErr(PaError &err);
  /* data */
public:
  fft fft_handler;

  void start_stream();
  port_audio_handler();
  ~port_audio_handler();
};