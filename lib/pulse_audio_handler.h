#include <iostream>
#include <pulse/pulseaudio.h>

struct sink_query {
  pa_threaded_mainloop *mloop;
  std::string sink_name;
  bool complete = false;
};

class pulse_audio_handler {
private:
  pa_threaded_mainloop *mloop;
  pa_mainloop_api *mlapi;
  pa_context *ctx;

  static void on_state_change(pa_context *c, void *userdata);
  static void on_server_info(pa_context *c, const pa_server_info *info,
                             void *userdata);
  static void on_sink_info(pa_context *c, const pa_sink_info *info, int eol,
                           void *userdata);

  std::string get_default_sink_name();
  std::string get_sink_monitor_name(std::string sink_name);

public:
  std::string get_default_audio_sink_monitor_name();
  pulse_audio_handler();
  ~pulse_audio_handler();
};
