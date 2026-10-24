#include "port_audio_handler.h"
#include <iostream>

void port_audio_handler::checkErr(PaError &err) {
  if (err != paNoError) {
    std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    exit(EXIT_FAILURE);
  }
}

port_audio_handler::port_audio_handler() {
  err = Pa_Initialize();
  const DefaultDeviceInfo default_device = initialize_default_device();
  checkErr(err);

  PaStreamParameters input{.device = default_device.id,
                           .channelCount = 2,
                           .sampleFormat = paFloat32,
                           .suggestedLatency = default_device.suggested_latency,
                           .hostApiSpecificStreamInfo = nullptr};

  err = Pa_OpenStream(&stream, &input, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                      paNoFlag, pa_callback, &fft_handler);
  checkErr(err);
  std::cout << "[ system ]: PortAudio stream opened" << std::endl;
}

void port_audio_handler::start_stream() {
  err = Pa_StartStream(stream);
  checkErr(err);
}

[[nodiscard]] DefaultDeviceInfo
port_audio_handler::initialize_default_device() {
  std::string sink_monitor =
      pulse_audio_h.get_default_audio_sink_monitor_name();

  int num_devices =
      Pa_GetDeviceCount(); // THIS IS PORT AUDIO METHOD :D NOT PULSE AUDIO

  if (num_devices < 0) {
    std::cout << "Error getting device count.";
    exit(EXIT_FAILURE);
  }

  if (num_devices == 0) {
    std::cout << "Zero available devices.";
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < num_devices; i++) {
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
    if (deviceInfo->name == sink_monitor) {
      return DefaultDeviceInfo{.sink_monitor = sink_monitor,
                               .id = i,
                               .suggested_latency =
                                   deviceInfo->defaultLowInputLatency};
    }
  }

  std::cout << "Error: device doesn't exist.";
  exit(EXIT_FAILURE);
}

int port_audio_handler::pa_callback(const void *inputBuffer, void *outputBuffer,
                                    unsigned long framesPerBuffer,
                                    const PaStreamCallbackTimeInfo *timeInfo,
                                    PaStreamCallbackFlags statusFlags,
                                    void *userData) {
  (void)outputBuffer;
  const float *inputAudioBuffer = static_cast<const float *>(inputBuffer);
  auto fft_handler = static_cast<fft *>(userData);
  fft_handler->apply_fft_on_window(inputAudioBuffer);

  return paContinue;
}

port_audio_handler::~port_audio_handler() {
  err = Pa_StopStream(stream);
  checkErr(err);
  err = Pa_CloseStream(stream);
  checkErr(err);
  err = Pa_Terminate();
  checkErr(err);
}