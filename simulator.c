#include <SDL2/SDL.h>
#include <SDL/SDL_ttf.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 20
#define MAIN_FONT "/user/share/fonts/TTF/DejaVuSans.ttf"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define SCALE 1
#define ROAD_WIDTH 150
#define LANE_WIDTH 50
#define ARROW_SIZE

const char* VEHICLE_FILE = "vehicle.data";

typedef struct{
    int currentLight;
    int nextLight;
}SharedData;

bool initializeSDL(SDL_Window **window, SDL_Renderer **renderer);
void drawRoadsAndLanes(SDL_Renderer *renderer, TTF_Font *font);
void displayText(SDL_Renderer *renderer, TTF_Font *font, char *text, int x, int y);
void drawLightForB(SDL_Renderer* renderer, bool isRed);
void refreshLight(SDL_Renderer *renderer, SharedData* SharedData);
void* chequeQueue(void* arg);
void* readAndParseFile(void* arg);

int main(){
    pthread_t tQueue, tReadFile;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    if(!initializeSDL(&window, &renderer)){
        return -1;
    }
    SDL_mutex* mutex = SDL_CreateMutex();
    SharedData sharedData = {0, 0};

    TTF_Font* font = TTF_OpenFont(MAIN_FONT, 24);
    if (!font) SDL_LOG("Failed to load font: %s", TTF_GetError());

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}