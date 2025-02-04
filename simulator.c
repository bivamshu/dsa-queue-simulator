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
    drawRoadsAndLane(renderer, font);
    SDL_RenderPresent(renderer);

    //we need to create seperate long running thread for the queue processing and light.
    //pthread_create(&tLight, NULL, refreshLight, &sharedData);
    pthread_create(&tQueue, NULL, refreashLight, &sharedData);
    pthread_create(&tReadFile, NULL, readAndParseFile, NULL);
    //readAndParseFile()

    //Continue the UUI thread 
    bool running = true;
    while(running){
        //update light
        refreshLight(renderer, &sharedData);
        while(SDL_PollEvent(&event))
            if(event.type == SDL_QUIT)
                running = false;
    }
    SDL_DestroyMutex(mutex);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(Window) SDL_DestroyWindow(window);
    //pthread_kill
    SDL_Quit();
    return 0;
}

bool initailizeSDL(SDL_Window **window, SDL_Renderer **renderer){
    if(SDL_init(SDL_INIT_VIDEO) < 0){
        SDL_LOG("Failed to initialize SDL: &s", SDL_GetError());
        return false;
    }
    //font init
    if(TTF_Init() < 0){
        SDL_Log("SDL_ttf could not initialize! TTF_Wrror: %s\n", TTF_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Junction Diagram",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWS_CENTERED,
    WINDOW_WIDTH*SCALE, WINDOW_HEIGHT*SCALE,
    SDL_WINDOW_SHOWN);
    
    if(!*window){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }
    return true;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void drawArrow(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3){
    //Sort vertices by ascending Y(bubble sort approach)
    if(y1 > y2) {swap(&y1, &y2); swap(&x1, &x2);}
    if(y1 > y3) {swap(&y1, &y3); swap(&x1, &x3);}
    if(y2 > y3) {swap(&y2, &y3); swap(&x2, &x3);}

    //Compute slopes
    float dx1 = (y2 - y1) ? (float)(x2 - x1) / (y2 - y1) : 0;
    float dx2 = (y3 - y1) ? (float)(x3 - x1) / (y3 - y1) : 0;
    float dx3 = (y3 - y1) ? (float)(x3 - x1) / (y3 - y1) : 0;

    float sx1 = x1, sx2 = x1;
    //Fill first part(top to middle)
    for(int y = y1; y<y2; y++){
        SDL_RenderDrawLine(renderer, (int)sx1, y, (int)sx2, y);
        sx1 += dx1;
        sx2 += dx2;
    }

    sx1 = x2;

    //Fill second part(middle to bottom)
    for(int y = y2; y <= y3; y++){
        SDL_RenderDrawLine(renderer, (int)sx1, y, (int)sx2, y);
        sx1 += dx3;
        sx2 += dx2;
    }
}


