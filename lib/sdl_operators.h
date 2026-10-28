#pragma once
#include <SDL3/SDL.h>

SDL_FPoint operator+(SDL_FPoint a, SDL_FPoint b);
SDL_FPoint operator-(SDL_FPoint a, SDL_FPoint b);
SDL_FPoint operator*(SDL_FPoint p, float scalar);
SDL_FPoint operator*(float scalar, SDL_FPoint p);
SDL_FPoint operator/(SDL_FPoint p, float scalar);
