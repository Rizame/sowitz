#include "sowitz.h"
#include "sdl_operators.h"

sowitz::sowitz(int WIDTH, int HEIGHT) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow("sowitz", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

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

std::vector<SDL_FPoint> sowitz::catmull_rom(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3,
                               SDL_FPoint p4) {
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
