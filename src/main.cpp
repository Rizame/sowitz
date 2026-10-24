#include "sowitz.h"
#include <cmath>
#include <stdlib.h>
// #include <iostream>
#include <SDL3/SDL.h>

#define WIDTH (800)
#define HEIGHT (500)
#define BIN_AMOUNT (std::floor(NUM_BINS / 2))
#define CX (WIDTH / 2)
#define CY (HEIGHT / 2)

float get_x(int i);

int main() {
  sowitz app(WIDTH, HEIGHT);
  app.start_application();

  while (app.running) {
    app.process_event();
    SDL_SetRenderDrawColor(app.renderer, 30, 30, 30, 255);
    SDL_RenderClear(app.renderer);
    SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
    const float *bins = app.get_audio_magnitude_values();

    // for (size_t i = 1; i < BIN_AMOUNT; i++) {
    //   float raw_mag = bins[i];

    //   float angle = 2 * M_PI * (log(BIN_FREQUENCY * i) - log(BIN_FREQUENCY))
    //   /
    //                     (log(BIN_FREQUENCY * BIN_AMOUNT) -
    //                     log(BIN_FREQUENCY)) -
    //                 M_PI / 2;

    //   app.display_magnitudes[i] =
    //       app.lerp(app.display_magnitudes[i], raw_mag, 0.1f, 1.0f);
    //   // std::cout << app.display_magnitudes[i] << std::endl;

    //   int to_x = CX + 500 * app.display_magnitudes[i] * cos(angle);
    //   int to_y = CY + 500 * app.display_magnitudes[i] * sin(angle);
    //   // int to_x = CX + 500 * raw_mag * cos(angle);
    //   // int to_y = CY + 500 * raw_mag * sin(angle);

    //   int color_id = std::floor(((255 / BIN_AMOUNT) * i));
    //   SDL_SetRenderDrawColor(app.renderer, 255, color_id, (color_id * 2 %
    //   255),
    //                          255);
    //   SDL_RenderLine(app.renderer, CX, CY, to_x, to_y);
    // }

    for (size_t i = 1; i < BIN_AMOUNT; i++) {
      float raw_mag = 5 + bins[i] * 600;
      float x = get_x(i);
      float x_next = i == BIN_AMOUNT - 1 ? WIDTH : get_x(i+1);
      app.display_magnitudes[i] =
          app.lerp(app.display_magnitudes[i], raw_mag, 0.1f, 1.0f);
      // x y w h

      float bin_width = x_next - x;
      SDL_FRect bin = {x, HEIGHT, bin_width, -app.display_magnitudes[i]};
      SDL_RenderFillRect(app.renderer, &bin);
    }

    SDL_RenderPresent(app.renderer);
    SDL_Delay(10);
  }
}

float get_x(int i) {
  return WIDTH * (log(BIN_FREQUENCY * i) - log(BIN_FREQUENCY)) /
         (log(BIN_FREQUENCY * BIN_AMOUNT) - log(BIN_FREQUENCY));
}

// while (running) {
//     while (SDL_PollEvent(&event)) {
//       if (event.type == SDL_EVENT_QUIT)
//         running = false;
//     }

//     SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
//     SDL_RenderClear(renderer);
//     SDL_SetRenderDrawColor(renderer, 230, 10, 10, 255);

//     float *bins = pa_handler.fft_handler.magnitude_values.load();

//     float bin_width = (float)WIDTH / BIN_AMOUNT;
//     for (size_t i = 0; i < BIN_AMOUNT; i++) {
//       float mag = 5 + bins[i] * 600;
//       SDL_FRect bin = {bin_width * i, HEIGHT - mag, bin_width, mag};
//       // x y w h
//       SDL_RenderFillRect(renderer, &bin);
//     }

//     SDL_RenderPresent(renderer);

//     SDL_Delay(10);
//   }

//   SDL_DestroyRenderer(renderer);
//   SDL_DestroyWindow(window);
//   SDL_Quit();
//
