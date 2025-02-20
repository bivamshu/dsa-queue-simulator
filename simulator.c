#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mutex.h>

#include <stdbool.h>
#include <pthread.h>
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>

#define MAX_LINE_LENGTH 20
#define MAIN_FONT "/usr/share/fonts/TTF/DejaVuSans.ttf"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define SCALE 1
#define ROAD_WIDTH 150
#define LANE_WIDTH 50
#define ARROW_SIZE 15

const char* VEHICLE_FILE = "vehicles.data";

typedef struct {
    int currentLight;
    int nextLight;
    SDL_mutex* mutex;
} SharedData;

// Function declarations
bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer);
void drawRoadsAndLane(SDL_Renderer *renderer, TTF_Font *font);
void displayText(SDL_Renderer *renderer, TTF_Font *font, char *text, int x, int y);
void drawLightForB(SDL_Renderer* renderer, bool isRed);
void refreshLight(SDL_Renderer *renderer, SharedData* sharedData);
void* chequeQueue(void* arg);
void* readAndParseFile(void* arg);

int main() {
    pthread_t tQueue, tReadFile;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;    
    SDL_Event event;    

    if (!initializeSDL(&window, &renderer)) return -1;

    SharedData sharedData = {0, 0, SDL_CreateMutex()};
    if (!sharedData.mutex) {
        SDL_Log("Failed to create mutex.");
        return -1;
    }

    TTF_Font* font = TTF_OpenFont(MAIN_FONT, 24);
    if (!font) SDL_Log("Failed to load font: %s", TTF_GetError());

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    drawRoadsAndLane(renderer, font);
    SDL_RenderPresent(renderer);

    pthread_create(&tQueue, NULL, chequeQueue, &sharedData);
    pthread_create(&tReadFile, NULL, readAndParseFile, NULL);

    // UI thread loop
    bool running = true;
    while (running) {
        SDL_LockMutex(sharedData.mutex);
        refreshLight(renderer, &sharedData);
        SDL_UnlockMutex(sharedData.mutex);

        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) running = false;
    }

    SDL_DestroyMutex(sharedData.mutex);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() < 0) return false;

    *window = SDL_CreateWindow("Junction Diagram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH*SCALE, WINDOW_HEIGHT*SCALE, SDL_WINDOW_SHOWN);
    if (!*window) return false;

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(*renderer, SCALE, SCALE);
    return *renderer != NULL;
}

void drawLightForB(SDL_Renderer* renderer, bool isRed) {
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_Rect lightBox = {400, 300, 50, 30};
    SDL_RenderFillRect(renderer, &lightBox);

    SDL_SetRenderDrawColor(renderer, isRed ? 255 : 11, isRed ? 0 : 156, isRed ? 0 : 50, 255);
    SDL_Rect light = {405, 305, 20, 20};
    SDL_RenderFillRect(renderer, &light);
}

void refreshLight(SDL_Renderer *renderer, SharedData* sharedData) {
    if (sharedData->nextLight == sharedData->currentLight) return;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    drawRoadsAndLane(renderer, TTF_OpenFont(MAIN_FONT, 24));
    drawLightForB(renderer, sharedData->nextLight == 2);
    SDL_RenderPresent(renderer);

    sharedData->currentLight = sharedData->nextLight;
}

void* chequeQueue(void* arg) {
    SharedData* sharedData = (SharedData*)arg;
    while (1) {
        SDL_LockMutex(sharedData->mutex);
        sharedData->nextLight = 0;
        SDL_UnlockMutex(sharedData->mutex);
        sleep(5);

        SDL_LockMutex(sharedData->mutex);
        sharedData->nextLight = 2;
        SDL_UnlockMutex(sharedData->mutex);
        sleep(5);
    }
}

void* readAndParseFile(void* arg) {
    while (1) {
        FILE* file = fopen(VEHICLE_FILE, "r");
        if (!file) {
            perror("Error opening file");
            sleep(2);
            continue;
        }

        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;
            char* vehicleNumber = strtok(line, ":");
            char* road = strtok(NULL, ":");
            if (vehicleNumber && road) printf("Vehicle: %s, Road: %s\n", vehicleNumber, road);
        }
        fclose(file);
        sleep(2);
    }
}
