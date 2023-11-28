#include <SDL2/SDL.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <stdio.h>

#include "log.hpp"

#define ARR_LEN (size_t)602

static void boardToSurface(const int *life_arr, int *pixels, size_t len)
{
    for (size_t i = 1; i < len - 1; i++)
    {
        for (size_t j = 1; j < len - 1; j++)
        {
            if (i == 0 || j == 0 || i == len - 1 || j == len - 1)
                *pixels = 0x00ffd700;
            else if (life_arr[i * len + j] == 0)
                *pixels = 0x00000000;
            else
                *pixels = 0x00ffcccc;
            pixels += 1;
        }
    }
}

static void fgen(const int *base_arr, int *cp_arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = 0; j < len; j++) cp_arr[i * len + j] = base_arr[i * len + j];
    }
}

static void gen(int *arr, size_t arr_len)
{
    arr[0]                       = arr[(arr_len - 1) * arr_len - 2];
    arr[arr_len - 1]             = arr[arr_len * (arr_len - 2) + 1];
    arr[arr_len * (arr_len - 1)] = arr[arr_len * 2 - 2];
    arr[arr_len * (arr_len - 1)] = arr[arr_len + 1];

    for (size_t i = 1; i < arr_len - 1; i++)
    {
        arr[i]                           = arr[arr_len * (arr_len - 2) + i];
        arr[arr_len * (arr_len - 1) + i] = arr[arr_len + i];

        arr[i * arr_len]                 = arr[i * arr_len + (arr_len - 2)];
        arr[i * arr_len + (arr_len - 1)] = arr[i * arr_len + 1];
    }
}

void uwgen(int *life_arr, int *temp_arr, size_t len)
{
    for (size_t i = 1; i < len - 1; i++)
    {
        for (size_t j = 1; j < len - 1; j++)
        {
            int sum = life_arr[i * len + j - 1] + life_arr[i * len + j + 1] + life_arr[(i - 1) * len + j] +
                      life_arr[(i + 1) * len + j] + life_arr[(i - 1) * len + j - 1] + life_arr[(i - 1) * len + j + 1] +
                      life_arr[(i + 1) * len + j - 1] + life_arr[(i + 1) * len + j + 1];

            if (sum == 3 || (sum == 2 && life_arr[i * len + j] == 1))
                temp_arr[i * len + j] = 1;
            else
                temp_arr[i * len + j] = 0;
        }
    }

    gen(temp_arr, len);
    fgen(temp_arr, life_arr, len);
}

static void populate(int *life_arr, size_t arr_len)
{
    for (size_t i = 0; i < arr_len; i++)
    {
        for (size_t j = 0; j < arr_len; j++) life_arr[i * arr_len + j] = rand() % 2;
    }

    gen(life_arr, arr_len);
}

int main(int argc, const char **argv)
{
    int            life_arr[ARR_LEN][ARR_LEN];
    int            temp_arr[ARR_LEN][ARR_LEN];
    unsigned short seed = 0;

    if (argc < 2)
    {
        Logger::warning("Using a random seed!");

        std::random_device                                       dev;
        std::mt19937                                             rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);

        seed = dist6(rng);
    }
    else
    {
        seed = std::atoi(argv[1]);
    }

    SDL_Window  *display;
    SDL_Surface *surface;

    char window_title[50];
    std::snprintf(window_title, 50, "Conway's Game of Life, Seed: %d", seed);

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        Logger::error("Could not initialize SDL Video");
        exit(-1);
    }

    display = SDL_CreateWindow(
        window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ARR_LEN - 2, ARR_LEN - 2, SDL_WINDOW_SHOWN);

    if (display == nullptr)
    {
        Logger::error("Could not initialize SDL Window");
        exit(-1);
    }

    surface     = SDL_GetWindowSurface(display);
    int *pixels = (int *)surface->pixels;

    populate(&life_arr[0][0], ARR_LEN);
    fgen(&life_arr[0][0], &temp_arr[0][0], ARR_LEN);
    boardToSurface(&life_arr[0][0], pixels, ARR_LEN);
    SDL_UpdateWindowSurface(display);

    SDL_Event event;

    for (;;)
    {
        if ((SDL_PollEvent(&event) != 0) && (event.type == SDL_QUIT)) break;

        uwgen(&life_arr[0][0], &temp_arr[0][0], ARR_LEN);
        boardToSurface(&life_arr[0][0], pixels, ARR_LEN);
        SDL_Delay(10);
        SDL_UpdateWindowSurface(display);
    }

    SDL_Quit();

    return 0;
}

