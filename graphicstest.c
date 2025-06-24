#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Point;

typedef struct {
    Point* data;
    size_t size;
    size_t capacity;
} PointArray;

typedef struct {
    int x;
    int y;
} Cursor;

void initPointArray(PointArray* arr) {
    arr->capacity = 4;
    arr->size = 0;
    arr->data = malloc(arr->capacity * sizeof(Point));
    if (!arr->data) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void addPoint(PointArray* arr, Point p) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(Point));
        if (!arr->data) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
    arr->data[arr->size++] = p;
}

void freePointArray(PointArray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = arr->capacity = 0;
}

int main() 
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(640 * 4, 480 * 4, 0, &window, &renderer) != 0) {
        fprintf(stderr, "SDL_CreateWindowAndRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_RenderSetScale(renderer, 4, 4);

    PointArray points;
    initPointArray(&points);

    Cursor cursor = {320, 240};  // Start in center

    int running = 1;
SDL_Event event;
int shiftPressed = 0;

while (running == 1) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                shiftPressed = 1;
            }
            else {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_x:
                        running = 0;
                        break;
                    case SDLK_w:
                        cursor.y--;
                        printf("cursor: X:%i Y:%i\n", cursor.x, cursor.y);
                        break;
                    case SDLK_a:
                        cursor.x--;
                        printf("cursor: X:%i Y:%i\n", cursor.x, cursor.y);
                        break;
                    case SDLK_s:
                        cursor.y++;
                        printf("cursor: X:%i Y:%i\n", cursor.x, cursor.y);
                        break;
                    case SDLK_d:
                        cursor.x++;
                        printf("cursor: X:%i Y:%i\n", cursor.x, cursor.y);
                        break;
                    case SDLK_SPACE:
                        if (shiftPressed) {
                            Point p = {cursor.x, cursor.y, 255, 255, 255, 255};
                            addPoint(&points, p);
                            printf("Added point at (%d, %d)\n", p.x, p.y);
                        }
                        break;
                    default:
                        printf("Other key pressed: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                        break;
                }
            }
        }
        else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                shiftPressed = 0;
            }
        }
    }  

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw points
    for (size_t i = 0; i < points.size; i++) {
        SDL_SetRenderDrawColor(renderer,
                               points.data[i].r,
                               points.data[i].g,
                               points.data[i].b,
                               points.data[i].a);
        SDL_RenderDrawPoint(renderer, points.data[i].x, points.data[i].y);
    }

    // Draw cursor as red point
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, cursor.x, cursor.y);

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}


    freePointArray(&points);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
