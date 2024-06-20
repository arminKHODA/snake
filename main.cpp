#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <sstream>

// Screen dimensions
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

// Grid dimensions
const int GRID_SIZE = 20;
const int BLOCK_SIZE = SCREEN_WIDTH / GRID_SIZE;

// Game speed (lower is faster)
const int GAME_SPEED = 150;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame();
    void run();

private:
    void init();
    void handleEvents();
    void update();
    void render();
    void reset();

    bool isCollision();
    void generateFood();

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    bool running;
    Direction dir;
    std::deque<Point> snake;
    Point food;
    int score;

    Uint32 lastTime, currentTime, elapsedTime;
};

SnakeGame::SnakeGame() : window(nullptr), renderer(nullptr), font(nullptr), running(false), dir(RIGHT), score(0), lastTime(0), currentTime(0), elapsedTime(0) {
    srand(static_cast<unsigned int>(time(0)));
    init();
}

void SnakeGame::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (TTF_Init() < 0) {
        std::cerr << "Could not initialize SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    window = SDL_CreateWindow("Snake",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    font = TTF_OpenFont("font.ttf", 24); // Provide the correct path to a TTF font file
    if (!font) {
        std::cerr << "Could not load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    reset();
}

void SnakeGame::reset() {
    snake.clear();
    snake.push_back({ GRID_SIZE / 2, GRID_SIZE / 2 });
    dir = RIGHT;
    score = 0;
    generateFood();
    running = true;
    lastTime = SDL_GetTicks();
}

void SnakeGame::generateFood() {
    food.x = rand() % GRID_SIZE;
    food.y = rand() % GRID_SIZE;
}

bool SnakeGame::isCollision() {
    Point head = snake.front();
    if (head.x < 0 || head.x >= GRID_SIZE || head.y < 0 || head.y >= GRID_SIZE) {
        return true;
    }
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].x == head.x && snake[i].y == head.y) {
            return true;
        }
    }
    return false;
}

void SnakeGame::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (dir != DOWN) dir = UP;
                break;
            case SDLK_DOWN:
                if (dir != UP) dir = DOWN;
                break;
            case SDLK_LEFT:
                if (dir != RIGHT) dir = LEFT;
                break;
            case SDLK_RIGHT:
                if (dir != LEFT) dir = RIGHT;
                break;
            }
        }
    }
}

void SnakeGame::update() {
    currentTime = SDL_GetTicks();
    elapsedTime = currentTime - lastTime;

    if (elapsedTime > GAME_SPEED) {
        Point newHead = snake.front();
        switch (dir) {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        }

        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            generateFood();
        }
        else {
            snake.pop_back();
        }

        snake.push_front(newHead);

        if (isCollision()) {
            running = false;
        }

        lastTime = currentTime;
    }
}

void SnakeGame::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (const Point& p : snake) {
        SDL_Rect block = { p.x * BLOCK_SIZE, p.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
        SDL_RenderFillRect(renderer, &block);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodBlock = { food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
    SDL_RenderFillRect(renderer, &foodBlock);

    std::stringstream scoreText;
    scoreText << "Score: " << score;
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.str().c_str(), { 255, 255, 255, 255 });
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect = { 10, 10, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void SnakeGame::run() {
    while (running) {
        handleEvents();
        update();
        render();
    }

    // Game Over screen
    while (true) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Game Over! Press R to Restart or ESC to Exit", { 255, 255, 255, 255 });
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destRect = { SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT / 2, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                    break;
                }
                else if (event.key.keysym.sym == SDLK_r) {
                    reset();
                    running = true;
                }
            }
        }

        if (!running) {
            break;
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    SnakeGame game;
    game.run();
    return 0;
}
