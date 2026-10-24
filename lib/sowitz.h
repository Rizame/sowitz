#include "port_audio_handler.h"
#include <SDL3/SDL.h>

class sowitz {
private:
  port_audio_handler pa_handler;
  SDL_Window *window;
  SDL_Event event;

  /* data */
public:
  float display_magnitudes[NUM_BINS]{0.01f};

  float lerp(float v0, float v1, float t, float coeff);
  void start_application();

  void process_event();
  const float *get_audio_magnitude_values();
  sowitz(int WIDTH, int HEIGHT);
  ~sowitz();

  bool running = false;
  SDL_Renderer *renderer;
};
