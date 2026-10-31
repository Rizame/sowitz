#include "port_audio_handler.h"
#include <SDL3/SDL.h>
#include <vector>

constexpr float FPS_RATE = 144.f;
constexpr float FRAME_TIME = 1000.f / FPS_RATE;

class sowitz {
private:
  int width;
  int height;
  port_audio_handler pa_handler;
  SDL_Window *window;
  SDL_Event event;

  /* data */
public:
  float get_bin_x_position(int i);
  float lerp(float v0, float v1, float t, float coeff);
  std::vector<SDL_FPoint> catmull_rom(SDL_FPoint p1, SDL_FPoint p2,
                                      SDL_FPoint p3, SDL_FPoint p4);
  void start_application();

  void process_event();
  const float *get_audio_magnitude_values();
  sowitz(int WIDTH, int HEIGHT);
  ~sowitz();

  bool running = false;
  SDL_Renderer *renderer;
  float display_magnitudes[TOTAL_BINS]{0.01f};
  SDL_FPoint mid_points[DISPLAY_BINS];
};
