#include "sdl_operators.h"

SDL_FPoint operator+(SDL_FPoint a, SDL_FPoint b) {
    return {a.x + b.x, a.y + b.y};
}

SDL_FPoint operator-(SDL_FPoint a, SDL_FPoint b) {
    return {a.x - b.x, a.y - b.y};
}

SDL_FPoint operator*(SDL_FPoint p, float scalar) {
    return {p.x * scalar, p.y * scalar};
}

SDL_FPoint operator*(float scalar, SDL_FPoint p) {
    return p * scalar;
}

SDL_FPoint operator/(SDL_FPoint p, float scalar) {
    return {p.x / scalar, p.y / scalar};
}
