// https://www.youtube.com/watch?v=KnCNfBb2ODQ
#include <SDL2/SDL.h>
#include <stdio.h>
#include <complex>
#include <iostream>

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900

using namespace std;

int WIDTH = 800;
int HEIGHT = 800;
int MAX_ITER = 25;
long double max_value = 1.0;
long double min_value = -2.84;
long double factor = 2;

long double scale(long double value,
                  long double max_old, long double min_old,
                  long double max_new, long double min_new)
{
    return ((value - min_old) /
            (max_old - min_old)) *
               (max_new - min_new) +
           min_new;
}

int julius(complex<long double> z, complex<long double> c)
{
    int iter = 0;
    for (int i = 0; i < MAX_ITER; i++)
    {
        z = z * z + c;
        iter++;
        if (abs(z) > 2)
        {
            return iter;
        }
    }
    return 0;
}

int mandlebort(complex<long double> c)
{
    complex<long double> z(0.0, 0.0);
    return julius(z, c);
}

int main(int argc, char *args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    bool quit = false;
    SDL_Event event;
    int count = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_RenderPresent(renderer);
        min_value *= 0.001;
        max_value *= 0.001;
        factor *= 0.94532;
        MAX_ITER += 5;
        cerr << "min value: " << min_value << '\n'
             << "max value: " << max_value << '\n'
             << "factor: " << factor << '\n'
             << "max iters: " << MAX_ITER << '\n';
        if (count > 20)
        {
            MAX_ITER *= 1.02;
        }
        count++;
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                long double scaled_x = scale(x, WIDTH, 0, max_value, min_value);
                long double scaled_y = scale(y, HEIGHT, 0, max_value, min_value);
                int iters = mandlebort(complex<long double>(scaled_x, scaled_y));
                int bright = scale(iters, MAX_ITER, 0, 255, 0);

                SDL_SetRenderDrawColor(renderer, bright, bright, bright, 255);
                SDL_RenderDrawPointF(renderer, x, y);
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
