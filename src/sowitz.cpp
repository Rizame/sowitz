#include "sowitz.h"
#include "sdl_operators.h"
#include <SDL3/SDL_render.h>

sowitz::sowitz(int WIDTH, int HEIGHT) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow("sowitz", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
  width = WIDTH;
  height = HEIGHT;
  if (!window) {
    std::cerr << SDL_GetError() << '\n';
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  renderer = SDL_CreateRenderer(window, nullptr);

  if (!renderer) {
    std::cerr << SDL_GetError() << '\n';
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  bool vsync = SDL_SetRenderVSync(renderer, 1);
  if (!vsync)
    std::cerr << "[ERROR]: couldn't enable the vsync" << std::endl;

  // first default zeroed point intialization:
  for (int i = 0; i < DISPLAY_BINS; i++) {
    mid_points[i] = {0.f, (float)HEIGHT - 5};
  }
}

void sowitz::start_application() {
  pa_handler.start_stream();
  running = true;
}

const float *sowitz::get_audio_magnitude_values() {
  return pa_handler.fft_handler.magnitude_values.load();
}

void sowitz::process_event() {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT)
      running = false;
  }
}

sowitz::~sowitz() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

float sowitz::lerp(float v0, float v1, float t, float coeff) {
  return v0 - v0 * t + v1 * t;
}

float sowitz::get_bin_x_position(int i) {
  return width * (log(BIN_FREQUENCY * i) - log(BIN_FREQUENCY)) /
         (log(BIN_FREQUENCY * DISPLAY_BINS) - log(BIN_FREQUENCY));
}

std::vector<SDL_FPoint> sowitz::catmull_rom(SDL_FPoint p1, SDL_FPoint p2,
                                            SDL_FPoint p3, SDL_FPoint p4) {
  std::vector<SDL_FPoint> points;
  for (float t = 0.0f; t <= 1.0f; t += 0.01f) {

    SDL_FPoint np1 = {
        .x = -p1.x,
        .y = -p1.y,
    };
    SDL_FPoint result = {
        .x = 0.f,
        .y = 0.f,
    };
    result = 0.5f * (2 * p2 + (np1 + p3) * t +
                     (2 * p1 - 5 * p2 + 4 * p3 - p4) * t * t +
                     (np1 + 3 * p2 - 3 * p3 + p4) * t * t * t);
    points.push_back(result);
  }

  return points;
}

//
