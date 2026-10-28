#include "port_audio_handler.h"
#include <SDL3/SDL.h>
#include <vector>

class sowitz {
private:
  port_audio_handler pa_handler;
  SDL_Window *window;
  SDL_Event event;

  /* data */
public:
  float display_magnitudes[NUM_BINS]{0.01f};

  float lerp(float v0, float v1, float t, float coeff);
  std::vector<SDL_FPoint> catmull_rom(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3,
                         SDL_FPoint p4);
  void start_application();

  void process_event();
  const float *get_audio_magnitude_values();
  sowitz(int WIDTH, int HEIGHT);
  ~sowitz();

  bool running = false;
  SDL_Renderer *renderer;
};
