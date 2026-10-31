#include "sowitz.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <stdlib.h>
// #include <iostream>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 500;

void render(sowitz &app) {
  SDL_SetRenderDrawColor(app.renderer, 30, 30, 30, 255);
  SDL_RenderClear(app.renderer);
  SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
  SDL_RenderLine(app.renderer, 0, HEIGHT, WIDTH, HEIGHT);

  // stopping one element earlier since catmull-rom spline
  // interpolates between i and i+1 using entering direction from i-1 and
  // exiting direction from i+2. No need to use last index.
  //
  // we connect points. there is always 1 connection less than there are points.
  for (int i = 0; i < DISPLAY_BINS - 1; i++) {
    std::vector<SDL_FPoint> points;
    if (i == 0) {
      points = app.catmull_rom(app.mid_points[i], app.mid_points[i],
                               app.mid_points[i + 1], app.mid_points[i + 2]);
    } else if (i == DISPLAY_BINS - 2) {
      points = app.catmull_rom(app.mid_points[i - 1], app.mid_points[i],
                               app.mid_points[i + 1], app.mid_points[i + 1]);
    } else {
      points = app.catmull_rom(app.mid_points[i - 1], app.mid_points[i],
                               app.mid_points[i + 1], app.mid_points[i + 2]);
    }
    SDL_RenderLines(app.renderer, points.data(), points.size());
    SDL_RenderLine(app.renderer, app.mid_points[i].x, app.mid_points[i].y,
                   app.mid_points[i].x, HEIGHT);
  }
  // std::cout << "renderer: " << app.renderer << std::endl;
  SDL_RenderPresent(app.renderer);
}

void update(sowitz &app) {
  app.process_event();
  const float *bins = app.get_audio_magnitude_values();

  // starting from i=1 because bin 0 is DC (0 Hz), nothing to compute here
  for (size_t i = 1; i < DISPLAY_BINS; i++) {
    float raw_mag = 5 + bins[i] * 600;
    float x = app.get_bin_x_position(i);
    float x_next =
        i == DISPLAY_BINS - 1 ? WIDTH : app.get_bin_x_position(i + 1);
    app.display_magnitudes[i] =
        app.lerp(app.display_magnitudes[i], raw_mag, 0.05f, 1.0f);
    // third parameter here is "progress" of an animation. Using 1.f would
    // mean each frame would draw the finished animation - 100% of its
    // timeline. 0.03f/FPS draws an animation that has moved for 3% from the
    // beginning. keeping it low makes the animation more smooth, slow and flowy

    app.mid_points[i] = {
        .x = x + (x_next - x) / 2,
        .y = HEIGHT - app.display_magnitudes[i],
    };
  }
}

int main() {
  sowitz app(WIDTH, HEIGHT);
  app.start_application();

  int frames = 0;
  Uint64 lastTime = 0;
  while (app.running) {
    Uint64 start = SDL_GetPerformanceCounter();
    update(app);
    render(app);

    frames++;
    Uint64 end = SDL_GetPerformanceCounter();
    // since vsync in enabled in SDL settings -> there is no need to manually
    // lock the frame rate it automatically locks at the highest frame rate
    if (start > lastTime + SDL_GetPerformanceFrequency()) {
      // count frames per second here
      std::cout << "\rFPS: " << frames << "    " << std::flush;
      frames = 0;
      lastTime = start;
    }
  }
}
