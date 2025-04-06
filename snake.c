#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240
#define GRID_SIZE 15
#define GRID_WIDTH (WINDOW_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / GRID_SIZE)

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point* body;
    int length;
    Point direction;
} Snake;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Snake snake;
Point food;
bool game_over = false;

void spawn_food(void);  // Prototype added here

void init_game() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    snake.body = malloc(sizeof(Point) * GRID_WIDTH * GRID_HEIGHT);
    snake.body[0].x = GRID_WIDTH / 2;
    snake.body[0].y = GRID_HEIGHT / 2;
    snake.length = 1;
    snake.direction.x = 1;
    snake.direction.y = 0;
    
    srand(time(NULL));
    spawn_food();
}

void spawn_food() {
    food.x = rand() % GRID_WIDTH;
    food.y = rand() % GRID_HEIGHT;
    for (int i = 0; i < snake.length; i++) {
        if (food.x == snake.body[i].x && food.y == snake.body[i].y) {
            spawn_food();
            return;
        }
    }
}

void handle_input() {
    SDL_Event event;
    bool direction_changed = false;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game_over = true;
        } else if (event.type == SDL_KEYDOWN && !direction_changed) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:    
                    if (snake.direction.y != 1) { 
                        snake.direction.x = 0; 
                        snake.direction.y = -1; 
                        direction_changed = true; 
                    } 
                    break;
                case SDLK_DOWN:  
                    if (snake.direction.y != -1) { 
                        snake.direction.x = 0; 
                        snake.direction.y = 1; 
                        direction_changed = true; 
                    } 
                    break;
                case SDLK_LEFT:  
                    if (snake.direction.x != 1) { 
                        snake.direction.x = -1; 
                        snake.direction.y = 0; 
                        direction_changed = true; 
                    } 
                    break;
                case SDLK_RIGHT: 
                    if (snake.direction.x != -1) { 
                        snake.direction.x = 1; 
                        snake.direction.y = 0; 
                        direction_changed = true; 
                    } 
                    break;
            }
        }
    }
}

void update_game() {
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0].x += snake.direction.x;
    snake.body[0].y += snake.direction.y;
    
    if (snake.body[0].x < 0 || snake.body[0].x >= GRID_WIDTH || 
        snake.body[0].y < 0 || snake.body[0].y >= GRID_HEIGHT) {
        game_over = true;
        return;
    }
    
    for (int i = 1; i < snake.length; i++) {
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
            game_over = true;
            return;
        }
    }
    
    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        if (snake.length > 1) {
            snake.body[snake.length] = snake.body[snake.length - 1];
        }
        snake.length++;
        spawn_food();
    }
}

void render_game() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snake.length; i++) {
        SDL_Rect rect = {snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect food_rect = {food.x * GRID_SIZE, food.y * GRID_SIZE, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &food_rect);
    
    SDL_RenderPresent(renderer);
}

int main() {
    init_game();
    
    while (!game_over) {
        handle_input();
        update_game();
        render_game();
        SDL_Delay(150);
    }
    
    free(snake.body);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
