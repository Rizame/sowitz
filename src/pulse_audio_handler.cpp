#include "pulse_audio_handler.h"

pulse_audio_handler::pulse_audio_handler() {
  this->mloop = pa_threaded_mainloop_new();
  if (pa_threaded_mainloop_start(this->mloop) < 0) {
    std::cout << "[ ERROR ]: Failed to start the PulseAudio main loop.";
    exit(EXIT_FAILURE);
  }
  pa_threaded_mainloop_lock(this->mloop);

  this->mlapi = pa_threaded_mainloop_get_api(this->mloop);
  this->ctx = pa_context_new_with_proplist(this->mlapi, "Aviz", NULL);

  pa_context_set_state_callback(this->ctx, pulse_audio_handler::on_state_change,
                                this->mloop);
  pa_context_connect(this->ctx, NULL, PA_CONTEXT_NOFLAGS, NULL);

  while (PA_CONTEXT_READY != pa_context_get_state(this->ctx)) {
    pa_threaded_mainloop_wait(this->mloop);
  }

  printf("[ system ]: Connected to PulseAudio server.\n");
}

pulse_audio_handler::~pulse_audio_handler() {
  pa_threaded_mainloop_unlock(this->mloop);
  pa_threaded_mainloop_stop(this->mloop);
  pa_threaded_mainloop_free(this->mloop);
}

std::string pulse_audio_handler::get_default_audio_sink_monitor_name() {
  std::string def_sink_name = this->get_default_sink_name();
  // we obtain default sink name here.

  std::string default_sink_monitor_name =
      this->get_sink_monitor_name(def_sink_name);
  // now we have the default sink monitor name. returning.

  return default_sink_monitor_name;
}

// private helpers

std::string pulse_audio_handler::get_default_sink_name() {
  sink_query sq = {.mloop = this->mloop};
  pa_context_get_server_info(this->ctx, this->on_server_info, &sq);
  while (!sq.complete) {
    pa_threaded_mainloop_wait(this->mloop);
  }
  return sq.sink_name;
}

std::string pulse_audio_handler::get_sink_monitor_name(std::string sink_name) {
  sink_query sq = {.mloop = this->mloop};
  pa_context_get_sink_info_by_name(this->ctx, sink_name.c_str(),
                                   this->on_sink_info, &sq);
  while (!sq.complete) {
    pa_threaded_mainloop_wait(this->mloop);
  }
  return sq.sink_name;
}

// CALLBACKS

void pulse_audio_handler::on_sink_info(pa_context *c, const pa_sink_info *info,
                                       int eol, void *userdata) {
  auto *query = static_cast<sink_query *>(userdata);
  if (eol) {
    query->complete = true;
    pa_threaded_mainloop_signal(query->mloop, 0);
    return;
  }
  query->sink_name = info->monitor_source_name;
}

void pulse_audio_handler::on_state_change(pa_context *c, void *userdata) {
  pa_threaded_mainloop_signal(static_cast<pa_threaded_mainloop *>(userdata), 0);
}

void pulse_audio_handler::on_server_info(pa_context *c,
                                         const pa_server_info *info,
                                         void *userdata) {
  auto *query = static_cast<sink_query *>(userdata);

  query->sink_name = info->default_sink_name;
  query->complete = true;

  pa_threaded_mainloop_signal(query->mloop, 0);
}