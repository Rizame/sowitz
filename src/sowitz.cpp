#include "sowitz.h"

sowitz::sowitz(int WIDTH, int HEIGHT) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow("Aviz", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

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


//
